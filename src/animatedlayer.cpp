/**
 * Copyright (C) 2012 by INdT
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * @author Rodrigo Goncalves de Oliveira <rodrigo.goncalves@openbossa.org>
 * @author Roger Felipe Zanoni da Silva <roger.zanoni@openbossa.org>
 */

#include "animatedlayer.h"

AnimatedLayer::AnimatedLayer(Layer *parent)
    : Layer((QuasiDeclarativeItem *)parent)
    , m_horizontalStep(0)
    , m_currentHorizontalStep(0)
{
}

AnimatedLayer::~AnimatedLayer()
{
}

void AnimatedLayer::setHorizontalStep(const qreal &step)
{
    int temp = step;
    if (temp < 0)
        temp *= -1;

    if (temp != m_horizontalStep) {
        m_horizontalStep = temp;

        emit horizontalStepChanged();
    }
}

void AnimatedLayer::updateHorizontalStep()
{
    int directionFactor = (m_direction == Quasi::ForwardDirection) ? 1 : -1;

    m_currentHorizontalStep += (m_horizontalStep * m_factor * directionFactor);

    if (m_currentHorizontalStep <= -width()) {
        drawPixmap();
        m_currentHorizontalStep = 0;
    } else if (m_currentHorizontalStep >= 0) {
        drawPixmap();
        m_currentHorizontalStep = -width();
    }
}

#if QT_VERSION >= 0x050000
void AnimatedLayer::paint(QPainter *painter)
{
#else
void AnimatedLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
#endif
    updateHorizontalStep();

    if (m_currentImage)
        painter->drawImage(m_currentHorizontalStep, 0, *m_currentImage);
}
