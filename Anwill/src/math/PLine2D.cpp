#include "PLine2D.h"

namespace Anwill::Math {

    PLine2D::PLine2D(Vec2f p, Vec2f d)
        : m_P(p), m_D(d)
    {}

    bool PLine2D::Intersects(const PLine2D &other, float& t1, float& t2)
    {
        t2 = (m_D.GetX() * (other.m_P.GetY() - m_P.GetY()) + m_D.GetY() * (m_P.GetX() - other.m_P.GetX())) /
                (other.m_D.GetX() * m_D.GetY() - other.m_D.GetY() * m_D.GetX());

        t1 = (other.m_P.GetX() + other.m_D.GetX() * t2 - m_P.GetX()) / m_D.GetX();

        if (t1 > 0 && (t2 > 0 && 1 > t2 )) {
            return true;
        }
        return false;
    }
}