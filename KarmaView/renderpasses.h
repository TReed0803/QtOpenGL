#ifndef RENDERPASSES_H
#define RENDERPASSES_H RenderPasses

enum RenderPassId
{
  GBufferPassId,
  PreparePresentationPassId,
  LightAccumulationPassId,
  ShadowedLightAccumulationPassId,
  MotionBlurPassId,
  ViewportPresentationPassId,
  DebugGBufferPassId,
  ScreenSpaceAmbientOcclusionPassId,
  EnvironmentPassId
};

#endif // RENDERPASSES_H
