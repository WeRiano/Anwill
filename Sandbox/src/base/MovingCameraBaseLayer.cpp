#include "MovingCameraBaseLayer.h"

MovingCameraBaseLayer::MovingCameraBaseLayer(unsigned int ups,
                                             const Anwill::WindowSettings& ws,
                                             float speed)
    : Anwill::Layer(ups), m_CameraController(ws.width, ws.height)
{}

void MovingCameraBaseLayer::Update(const Anwill::Timestamp& timestamp)
{
    m_CameraController.PollMovement(GetUpdateDelta(timestamp));
    Layer::Update(timestamp);
}