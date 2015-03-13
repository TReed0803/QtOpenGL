#ifndef RENDERPASSES_H
#define RENDERPASSES_H RenderPasses

enum RenderPassId
{
  GBufferPassId,
  PreparePresentationPassId,
  LightAccumulationPassId,
  CompositionPassId,
  MotionBlurPassId,
  ViewportPresentationPassId
};

#endif // RENDERPASSES_H
