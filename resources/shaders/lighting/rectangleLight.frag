/*******************************************************************************
 * lighting/pointLight.frag
 *------------------------------------------------------------------------------
 * Apply the lighting calculation to a given fragment of incident light.
 * Uses GBuffer information to access statistics about the scene itself.
 ******************************************************************************/

#include <GBuffer.ubo>
#include <Math.glsl> // saturate
#include <Physical.glsl>
#include <AreaLightBuffer.ubo>

vec3 getSpecularDominantDirArea(vec3 N, vec3 R, float Krough)
{
  return normalize(mix(N, R, 1.0 - Krough));
}

layout (std430, binding = 10) buffer BufferObject
{
  ivec4 SamplePosition;
  vec4 WorldPosition;
  vec4 ViewVector;
  vec4 ReflectionVector;
  vec4 NormalVector;
  vec4 MrpPoint;
  vec4 MrpUnclamped;
  vec4 EllipseMajor;
  vec4 EllipseMinor;
  vec4 EllipseNormal;
  vec4 BottomLeft;
  vec4 TopRight;
  vec4 BottomRight;
  vec4 TopLeft;
} Results;

float rectangleSolidAngle(vec3 viewPos, vec3 p0, vec3 p1, vec3 p2 , vec3 p3 )
{
  vec3 v0 = p0 - viewPos;
  vec3 v1 = p1 - viewPos;
  vec3 v2 = p2 - viewPos;
  vec3 v3 = p3 - viewPos;

  vec3 n0 = normalize( cross(v0, v1 ) );
  vec3 n1 = normalize( cross(v1, v2 ) );
  vec3 n2 = normalize( cross(v2, v3 ) );
  vec3 n3 = normalize( cross(v3, v0 ) );

  float g0 = acos( dot(-n0, n1 ) );
  float g1 = acos( dot(-n1, n2 ) );
  float g2 = acos( dot(-n2, n3 ) );
  float g3 = acos( dot(-n3, n0 ) );

  return g0 + g1 + g2 + g3 - 2 * pi;
}


// Light Output
layout(location = 0) out vec4 fFragColor;

float smoothDistanceAtt(float squaredDistance, float invSqrAttRadius)
{
  float factor = squaredDistance * invSqrAttRadius;
  float smoothFactor = saturate(1.0 - factor * factor);
  return smoothFactor * smoothFactor;
}

float getDistanceAtt(float sqrDist, float invSqrAttRadius)
{
  float attenuation = 1.0 / max( sqrDist , 0.0001);
  attenuation *= smoothDistanceAtt(sqrDist, invSqrAttRadius);
  return attenuation;
}

vec3 rayPlaneIntersect(vec3 rayOrigin, vec3 rayDirection, vec3 planeOrigin, vec3 planeNormal)
{
  float dist = dot(planeNormal, planeOrigin - rayOrigin) /dot(planeNormal , rayDirection);
  return rayOrigin + rayDirection * dist;
}

vec3 closestPointRect(vec3 pos, vec3 planeOrigin, vec3 right, vec3 up, float hWidth, float hHeight)
{
  vec3 dir = pos - planeOrigin;
  vec2 dist2D = vec2(dot(dir, right), dot(dir, up));
  vec2 halfDim = vec2(hWidth, hHeight);
  dist2D = clamp(dist2D, -halfDim, halfDim);
  return planeOrigin + dist2D.x * right + dist2D.y * up;
}

vec3 smallestAnglePointRect(vec3 sNorm, vec3 pNorm, vec3 sPos, vec3 pRight, vec3 pUp, float hWidth, float hHeight)
{
  float clampCosAngle = 0.001 + saturate(dot(sNorm, pNorm));
  // clamp d0 to the positive hemisphere of surface normal
  vec3 d0 = normalize(-pNorm + sNorm * clampCosAngle);
  // clamp d1 to the negative hemisphere of light plane normal
  vec3 d1 = normalize(sNorm - pNorm * clampCosAngle);
  vec3 dh = normalize(d0 + d1);
  vec3 ph = rayPlaneIntersect(sPos, dh, AreaLight.ViewPosition, pNorm) ;
  return closestPointRect(ph, AreaLight.ViewPosition, pRight, pUp, hWidth, hHeight);
}

void main()
{
  // GBuffer Access
  vec2 halfDim  = AreaLight.Data0.xy;
  vec2 dims     = AreaLight.Data0.zw;
  float width   = dims.x;
  float height  = dims.y;
  float halfWidth = halfDim.x;
  float halfHeight = halfDim.y;
  vec3 forward  = AreaLight.Data1.xyz;
  vec3 right    = AreaLight.Data2.xyz;
  vec3 up       = AreaLight.Data3.xyz;
  vec3 viewPos  = viewPosition();
  vec3 normal   = normal();
  vec3 baseColor= baseColor();

  // Light Information
  vec3  lightVec    = AreaLight.ViewPosition - viewPos;
  float sqrLightDist= dot(lightVec, lightVec);
  float lightDist   = sqrt(sqrLightDist);
  vec3  viewDir     = normalize(-viewPos);
  vec3  lightDir    = lightVec / lightDist;
  vec3  r           = normalize(reflect(viewDir, normal));
  vec3  r2          = getSpecularDominantDirArea(normal, r, roughness());
  float sqrLightRadius = AreaLight.Intensity;//AreaLight.Radius * AreaLight.Radius;
  vec3 lightPlaneNormal = forward;
  vec3 lightViewPos = AreaLight.ViewPosition;
  vec3 lightRight = right;
  vec3 lightUp = up;

  // From the equation: Luminance = φ / (Aπ);
  float luminance   = AreaLight.Intensity / (width * height * pi);
  // Inverse Square for attenuation Intensity = Luminance / (dist * dist)
  float invSqrAttRadius = 1.0 / sqrLightRadius;
  // Smooth attenuation formed by inverse square
  float attenuation = getDistanceAtt(sqrLightDist, invSqrAttRadius);
  //attenuation = pow(saturate(1.0 - pow(sqrLightDist / sqrLightRadius, 2.0)), 2.0) / (sqrLightDist + 1.0);


  // Our ray is represented as: t*viewDir, we're trying to solve for t.
  // A plane may be represented as: origin + u*right + v*up
  // So we want to solve origin = u*right + v*up + t*view
  // Which can be solved by: inverse([ [right], [up], [view]]) * origin = [u, v, t]
  vec3 viewRay           = normalize(viewPos);
  mat3 planeIntersection = inverse(mat3(right, up, viewRay));
  vec3 intersectionParams= planeIntersection * AreaLight.ViewPosition;

  // We also want to calculate the MRP for the area (Most Representative Point)
  // This can be accomplished through similar means, but instead with the r vector.
  float FoR = dot(forward, r);
  /*
  if (FoR >= 0.0)
  {
    r = reflect(r, forward);
  }
  */
  vec3 viewPosPlaneRef = viewPos - AreaLight.ViewPosition;
  planeIntersection = inverse(mat3(right, up, r));
  vec3 parametrics  = planeIntersection * viewPosPlaneRef;
  vec3 mrpUnclamped = AreaLight.ViewPosition + parametrics.x * right + parametrics.y * up;
  vec3 mrpPoint     = AreaLight.ViewPosition +
    clamp(parametrics.x, -halfWidth , halfWidth ) * right +
    clamp(parametrics.y, -halfHeight, halfHeight) * up;

  /*

  float minorRadius = abs(parametrics.z * tan(0.25));
  vec3 minorAxis = normalize(cross(forward, r));
  vec3 majorAxis = normalize(cross(minorAxis, forward));
  float majorRadius = abs(minorRadius * tan(dot(forward, r)));
  vec3 majorLeft = mrpUnclamped - majorRadius * minorAxis;
  vec3 majorRight = mrpUnclamped + majorRadius * minorAxis;
  vec3 minorUp = mrpUnclamped + minorRadius * minorAxis;
  vec3 minorDown = mrpUnclamped - minorRadius * minorAxis;
  float ellipseArea = pi * majorRadius * minorRadius;
  float majorPortion = majorRadius / (minorRadius + majorRadius);
  float majorEllipse = majorRadius * pi / 4.0;
  float minorEllipse = minorRadius * pi / 4.0;
  float verticalAdj = majorEllipse * abs(dot(up, majorAxis)) + minorEllipse * abs(dot(up, minorAxis));
  float horizontalAdj = majorEllipse * abs(dot(right, majorAxis)) + minorEllipse * abs(dot(right, minorAxis));
  vec3 blPoint = mrpUnclamped - verticalAdj * up - horizontalAdj * right;
  vec3 trPoint = mrpUnclamped + verticalAdj * up + horizontalAdj * right;
  vec3 brPoint = mrpUnclamped - verticalAdj * up + horizontalAdj * right;
  vec3 tlPoint = mrpUnclamped + verticalAdj * up - horizontalAdj * right;
  */

  vec3 p0 = lightViewPos + lightRight *  halfWidth + lightUp *  halfHeight;
  vec3 p1 = lightViewPos + lightRight *  halfWidth + lightUp * -halfHeight;
  vec3 p2 = lightViewPos + lightRight * -halfWidth + lightUp * -halfHeight;
  vec3 p3 = lightViewPos + lightRight * -halfWidth + lightUp *  halfHeight;

  // Debug Lighting
  if (abs(intersectionParams.x) < halfWidth  &&
      abs(intersectionParams.y) < halfHeight &&
      intersectionParams.z > 0.0)
  {
    fFragColor = vec4(AreaLight.Color * 1000.0, 1.0);
  }
  else
  {
    fFragColor = vec4(0.0, 0.0, 0.0, 1.0);

    // Illuminance (Correct horizon handling)
    vec3 lightPlaneNormal = forward;
    vec3 lightViewPos = AreaLight.ViewPosition;
    vec3 lightRight = right;
    vec3 lightUp = up;
    float illuminance = 0.0;
    float specular = 0.0;
    float NoL = 0.0;
    if ( dot(viewPos - lightViewPos, lightPlaneNormal) > 0)
    {
      float solidAngle = rectangleSolidAngle(viewPos, p0, p1, p2, p3);

      illuminance = solidAngle * 0.2 * (
        saturate( dot( normalize( p0 - viewPos          ), normal ) ) +
        saturate( dot( normalize( p1 - viewPos          ), normal ) ) +
        saturate( dot( normalize( p2 - viewPos          ), normal ) ) +
        saturate( dot( normalize( p3 - viewPos          ), normal ) ) +
        saturate( dot( normalize( lightViewPos - viewPos), normal ) ) );
      illuminance *= pi * luminance;

      vec3 closestPoint = mrpPoint - viewPos;
      vec3 l = normalize(closestPoint);

      vec3    H = normalize(l + viewDir);
      NoL = saturate(dot(normal, l));
      float NoR = saturate(dot(-r, l));
      float NoV = saturate(dot(normal, viewDir));
      float NoH = saturate(dot(normal, H));
      float VoH = saturate(dot(viewDir, H));
      specular = saturate(Specular(NoL, NoV, NoH, VoH));
      specular *= NoL * roughness();
    }
    vec3 KDiff = AreaLight.Color * max(baseColor * illuminance * attenuation, 0.0);
    vec3 KSpec = AreaLight.Color * max(specular * luminance * 0.5, 0.0);

    fFragColor = vec4(BlendMaterial(KDiff, KSpec), 1.0);
  }
  if (FoR >= 0.0)
  {
    //fFragColor += vec4(100.0, 0.0, 0.0, 1.0);
  }

  // Output into SSBO for debugging
  /*
  if (ivec2(gl_FragCoord.xy) == Results.SamplePosition.xy)
  {
    Results.WorldPosition = (Current.ViewToWorld * vec4(viewPos, 1.0));
    Results.ViewVector = (Current.ViewToWorld * vec4(viewDir, 0.0));
    Results.ReflectionVector = (Current.ViewToWorld * vec4(r, 0.0));
    Results.MrpPoint = (Current.ViewToWorld * vec4(mrpPoint, 1.0));
    Results.NormalVector = (Current.ViewToWorld * vec4(normal, 0.0));
    Results.MrpUnclamped = (Current.ViewToWorld * vec4(mrpUnclamped, 1.0));
    Results.EllipseMinor = (Current.ViewToWorld * vec4(minorRadius * minorAxis, 0.0));
    Results.EllipseMajor = (Current.ViewToWorld * vec4(majorRadius * majorAxis, 0.0));
    Results.EllipseNormal = (Current.ViewToWorld * vec4(forward, 0.0));
    Results.BottomLeft = (Current.ViewToWorld * vec4(blPoint, 1.0));
    Results.TopRight = (Current.ViewToWorld * vec4(trPoint, 1.0));
    Results.BottomRight = (Current.ViewToWorld * vec4(brPoint, 1.0));
    Results.TopLeft = (Current.ViewToWorld * vec4(tlPoint, 1.0));
  }
  */
}
