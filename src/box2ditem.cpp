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

#include "box2ditem.h"

#include "material.h"
#include "box2dscene.h"
#include "fixture.h"
#include "shape.h"
#include "util.h"

#include <Box2D/Box2D.h>

Box2DItem::Box2DItem(Scene *parent)
    : Box2DBaseItem(parent)
    , m_body(0)
    , m_linearDamping(0.0f)
    , m_angularDamping(0.0f)
    , m_bodyType(Quasi::DynamicBodyType)
    , m_bullet(false)
    , m_sleepingAllowed(true)
    , m_fixedRotation(false)
    , m_active(true)
{
    setTransformOrigin(Center);
    connect(this, SIGNAL(rotationChanged()), SLOT(onRotationChanged()));
}

Box2DItem::~Box2DItem()
{
    if (!m_world || !m_body)
        return;

#if QT_VERSION >= 0x050000
    QQuickItem *child;
#else
    QGraphicsItem *child;
#endif

    foreach (child, childItems())
        if (Fixture *fixture = dynamic_cast<Fixture *>(child))
            delete fixture;

    m_worldPtr->DestroyBody(m_body);
    m_body = 0;
}

b2Body *Box2DItem::body() const
{
    return m_body;
}

void Box2DItem::onRotationChanged()
{
    if (!m_synchronizing && m_body) {
        m_body->SetTransform(m_body->GetPosition(),
                             (rotation() * 2 * b2_pi) / -360.0);
    }
}

/*
 * Shamelessly stolen from qml-box2d project at gitorious
 *
 * https://gitorious.org/qml-box2d/qml-box2d
 */
void Box2DItem::initialize()
{
    if (m_initialized || !m_world)
        return;

    b2BodyDef bodyDef;
    bodyDef.type = static_cast<b2BodyType>(m_bodyType);
    bodyDef.position.Set((x() + width() / 2.0) / m_scaleRatio,
                         (-y() - height() / 2.0) / m_scaleRatio);

    bodyDef.angle = -(rotation() * (2 * b2_pi)) / 360.0;
    bodyDef.linearDamping = m_linearDamping;
    bodyDef.angularDamping = m_angularDamping;
    bodyDef.bullet = m_bullet;
    bodyDef.allowSleep = m_sleepingAllowed;
    bodyDef.fixedRotation = m_fixedRotation;

    m_body = m_worldPtr->CreateBody(&bodyDef);

    initializeFixtures();

    m_initialized = true;
}

qreal Box2DItem::linearDamping() const
{
    return m_linearDamping;
}

void Box2DItem::setLinearDamping(const qreal &linearDamping)
{
    if (m_linearDamping != linearDamping) {
        m_linearDamping = linearDamping;

        if (m_body)
            m_body->SetLinearDamping(linearDamping);

        emit linearDampingChanged();
    }
}

qreal Box2DItem::angularDamping() const
{
    return m_angularDamping;
}

void Box2DItem::setAngularDamping(const qreal &angularDamping)
{
    if (m_angularDamping != angularDamping) {
        m_angularDamping = angularDamping;

        if (m_body)
            m_body->SetAngularDamping(angularDamping);

        emit angularDampingChanged();
    }
}

Quasi::BodyType Box2DItem::bodyType() const
{
    return m_bodyType;
}

void Box2DItem::setBodyType(const Quasi::BodyType &bodyType)
{
    if (m_bodyType != bodyType) {
        m_bodyType = bodyType;

        if (m_body)
            m_body->SetType((b2BodyType)bodyType);

        emit bodyTypeChanged();
    }
}

bool Box2DItem::bullet() const
{
    return m_bullet;
}

void Box2DItem::setBullet(const bool &bullet)
{
    if (m_bullet != bullet) {
        m_bullet = bullet;

        if (m_body)
            m_body->SetBullet(bullet);

        emit bulletChanged();
    }
}

bool Box2DItem::sleepingAllowed() const
{
    return m_sleepingAllowed;
}

void Box2DItem::setSleepingAllowed(const bool &sleepingAllowed)
{
    if (m_sleepingAllowed != sleepingAllowed) {
        m_sleepingAllowed = sleepingAllowed;

        if (m_body)
            m_body->SetSleepingAllowed(sleepingAllowed);

        emit sleepingAllowedChanged();
    }
}

bool Box2DItem::fixedRotation() const
{
    return m_fixedRotation;
}

void Box2DItem::setFixedRotation(const bool &fixedRotation)
{
    if (m_fixedRotation != fixedRotation) {
        m_fixedRotation = fixedRotation;

        if (m_body)
            m_body->SetFixedRotation(fixedRotation);

        emit fixedRotationChanged();
    }
}

bool Box2DItem::active() const
{
    return m_active;
}

void Box2DItem::setActive(const bool &active)
{
    if (m_active != active) {
        m_active = active;

        if (m_body)
            m_body->SetActive(active);

        emit activeChanged();
    }
}

void Box2DItem::applyTorque(const float &torque)
{
    if (m_body)
        m_body->ApplyTorque(torque);
}

void Box2DItem::applyLinearImpulse(const QPointF &impulse, const QPointF &point)
{
    if (m_body) {
        m_body->ApplyLinearImpulse(b2Vec2(impulse.x() / m_scaleRatio,
                                          -impulse.y() / m_scaleRatio),
                                   b2Vec2(point.x() / m_scaleRatio,
                                          -point.y() / m_scaleRatio));
    }
}

void Box2DItem::setLinearVelocity(const QPointF &velocity)
{
    if (m_body) {
        m_body->SetLinearVelocity(b2Vec2(velocity.x() / m_scaleRatio,
                                         -velocity.y() / m_scaleRatio));
    }
}

void Box2DItem::setAngularVelocity(const float &velocity)
{
    if (m_body) {
        m_body->SetAngularVelocity(velocity);
    }
}

void Box2DItem::geometryChanged(const QRectF &newGeometry,
                                const QRectF &oldGeometry)
{
    if (!m_synchronizing && m_body) {
        if (newGeometry.topLeft() != oldGeometry.topLeft()) {
            const QPointF pos = newGeometry.topLeft();

            m_body->SetTransform(b2Vec2((pos.x() + width() / 2.0) / m_scaleRatio,
                                        (-pos.y() - height() / 2.0) / m_scaleRatio),
                                 m_body->GetAngle());
        }
    }

#if QT_VERSION >= 0x050000
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
#else
    QDeclarativeItem::geometryChanged(newGeometry, oldGeometry);
#endif
}

b2Vec2 Box2DItem::b2TransformOrigin() const
{
    b2Vec2 vec;
    if (m_body)
        vec = m_body->GetPosition();

    return vec;
}

float Box2DItem::b2Angle() const
{
    float32 angle = 0.0f;
    if (m_body)
        angle = m_body->GetAngle();
    return angle;
}

void Box2DItem::initializeFixtures()
{
#if QT_VERSION >= 0x050000
    QQuickItem *item;
#else
    QGraphicsItem *item;
#endif

    foreach (item, childItems()) {
        if (Fixture *fixture = dynamic_cast<Fixture *>(item)) {
            fixture->setWorld(m_world);
            fixture->setBody(this);
            fixture->initialize();
        }
    }
}

#if QT_VERSION >= 0x050000
void Box2DItem::itemChange(ItemChange change, const ItemChangeData &data)
#else
QVariant Box2DItem::itemChange(GraphicsItemChange change, const QVariant &value)
#endif
{
    if (isComponentComplete() && change == ItemChildAddedChange) {
#if QT_VERSION >= 0x050000
        QQuickItem *child = data.item;
#else
        QGraphicsItem *child = value.value<QGraphicsItem *>();
#endif
        if (Fixture *fixture = dynamic_cast<Fixture *>(child)) {
            fixture->setWorld(m_world);
            fixture->setBody(this);
            fixture->initialize();
        }
    }

#if QT_VERSION >= 0x050000
    Box2DBaseItem::itemChange(change, data);
#else
    return Box2DBaseItem::itemChange(change, value);
#endif
}
