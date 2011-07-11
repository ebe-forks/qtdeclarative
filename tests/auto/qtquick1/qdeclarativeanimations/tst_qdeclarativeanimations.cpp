/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <qtest.h>
#include <QtDeclarative/qdeclarativeengine.h>
#include <QtDeclarative/qdeclarativecomponent.h>
#include <QtQuick1/qdeclarativeview.h>
#include <QtQuick1/private/qdeclarativerectangle_p.h>
#include <QtQuick1/private/qdeclarativeanimation_p.h>
#include <QtQuick1/private/qdeclarativeitem_p.h>
#include <QVariantAnimation>
#include <QEasingCurve>

#include "../../../shared/util.h"

#ifdef Q_OS_SYMBIAN
// In Symbian OS test data is located in applications private dir
#define SRCDIR "."
#endif

class tst_qdeclarativeanimations : public QObject
{
    Q_OBJECT
public:
    tst_qdeclarativeanimations() {}

private slots:
    void initTestCase() { QDeclarativeEngine engine; } // ensure types are registered

    void simpleProperty();
    void simpleNumber();
    void simpleColor();
    void simpleRotation();
    void alwaysRunToEnd();
    void complete();
    void resume();
    void dotProperty();
    void badTypes();
    void badProperties();
    void mixedTypes();
    void properties();
    void propertiesTransition();
    void invalidDuration();
    void attached();
    void propertyValueSourceDefaultStart();
    void dontStart();
    void easingProperties();
    void rotation();
    void runningTrueBug();
    void nonTransitionBug();
    void registrationBug();
    void doubleRegistrationBug();
    void alwaysRunToEndRestartBug();
};

#define QTIMED_COMPARE(lhs, rhs) do { \
    for (int ii = 0; ii < 5; ++ii) { \
        if (lhs == rhs)  \
            break; \
        QTest::qWait(50); \
    } \
    QCOMPARE(lhs, rhs); \
} while (false)

void tst_qdeclarativeanimations::simpleProperty()
{
    QDeclarative1Rectangle rect;
    QDeclarative1PropertyAnimation animation;
    animation.setTarget(&rect);
    animation.setProperty("pos");
    animation.setTo(QPointF(200,200));
    QVERIFY(animation.target() == &rect);
    QVERIFY(animation.property() == "pos");
    QVERIFY(animation.to().toPointF() == QPointF(200,200));
    animation.start();
    QVERIFY(animation.isRunning());
    QTest::qWait(animation.duration());
    QTIMED_COMPARE(rect.pos(), QPointF(200,200));

    rect.setPos(0,0);
    animation.start();
    animation.pause();
    QVERIFY(animation.isRunning());
    QVERIFY(animation.isPaused());
    animation.setCurrentTime(125);
    QVERIFY(animation.currentTime() == 125);
    QCOMPARE(rect.pos(), QPointF(100,100));
}

void tst_qdeclarativeanimations::simpleNumber()
{
    QDeclarative1Rectangle rect;
    QDeclarative1NumberAnimation animation;
    animation.setTarget(&rect);
    animation.setProperty("x");
    animation.setTo(200);
    QVERIFY(animation.target() == &rect);
    QVERIFY(animation.property() == "x");
    QVERIFY(animation.to() == 200);
    animation.start();
    QVERIFY(animation.isRunning());
    QTest::qWait(animation.duration());
    QTIMED_COMPARE(rect.x(), qreal(200));

    rect.setX(0);
    animation.start();
    animation.pause();
    QVERIFY(animation.isRunning());
    QVERIFY(animation.isPaused());
    animation.setCurrentTime(125);
    QVERIFY(animation.currentTime() == 125);
    QCOMPARE(rect.x(), qreal(100));
}

void tst_qdeclarativeanimations::simpleColor()
{
    QDeclarative1Rectangle rect;
    QDeclarative1ColorAnimation animation;
    animation.setTarget(&rect);
    animation.setProperty("color");
    animation.setTo(QColor("red"));
    QVERIFY(animation.target() == &rect);
    QVERIFY(animation.property() == "color");
    QVERIFY(animation.to() == QColor("red"));
    animation.start();
    QVERIFY(animation.isRunning());
    QTest::qWait(animation.duration());
    QTIMED_COMPARE(rect.color(), QColor("red"));

    rect.setColor(QColor("blue"));
    animation.start();
    animation.pause();
    QVERIFY(animation.isRunning());
    QVERIFY(animation.isPaused());
    animation.setCurrentTime(125);
    QVERIFY(animation.currentTime() == 125);
    QCOMPARE(rect.color(), QColor::fromRgbF(0.498039, 0, 0.498039, 1));

    rect.setColor(QColor("green"));
    animation.setFrom(QColor("blue"));
    QVERIFY(animation.from() == QColor("blue"));
    animation.restart();
    QCOMPARE(rect.color(), QColor("blue"));
    QVERIFY(animation.isRunning());
    animation.setCurrentTime(125);
    QCOMPARE(rect.color(), QColor::fromRgbF(0.498039, 0, 0.498039, 1));
}

void tst_qdeclarativeanimations::simpleRotation()
{
    QDeclarative1Rectangle rect;
    QDeclarative1RotationAnimation animation;
    animation.setTarget(&rect);
    animation.setProperty("rotation");
    animation.setTo(270);
    QVERIFY(animation.target() == &rect);
    QVERIFY(animation.property() == "rotation");
    QVERIFY(animation.to() == 270);
    QVERIFY(animation.direction() == QDeclarative1RotationAnimation::Numerical);
    animation.start();
    QVERIFY(animation.isRunning());
    QTest::qWait(animation.duration());
    QTIMED_COMPARE(rect.rotation(), qreal(270));

    rect.setRotation(0);
    animation.start();
    animation.pause();
    QVERIFY(animation.isRunning());
    QVERIFY(animation.isPaused());
    animation.setCurrentTime(125);
    QVERIFY(animation.currentTime() == 125);
    QCOMPARE(rect.rotation(), qreal(135));
}

void tst_qdeclarativeanimations::alwaysRunToEnd()
{
    QDeclarative1Rectangle rect;
    QDeclarative1PropertyAnimation animation;
    animation.setTarget(&rect);
    animation.setProperty("x");
    animation.setTo(200);
    animation.setDuration(1000);
    animation.setLoops(-1);
    animation.setAlwaysRunToEnd(true);
    QVERIFY(animation.loops() == -1);
    QVERIFY(animation.alwaysRunToEnd() == true);
    animation.start();
    QTest::qWait(1500);
    animation.stop();
    QVERIFY(rect.x() != qreal(200));
    QTest::qWait(500);
    QTIMED_COMPARE(rect.x(), qreal(200));
}

void tst_qdeclarativeanimations::complete()
{
    QDeclarative1Rectangle rect;
    QDeclarative1PropertyAnimation animation;
    animation.setTarget(&rect);
    animation.setProperty("x");
    animation.setFrom(1);
    animation.setTo(200);
    animation.setDuration(500);
    QVERIFY(animation.from() == 1);
    animation.start();
    QTest::qWait(50);
    animation.stop();
    QVERIFY(rect.x() != qreal(200));
    animation.start();
    QTest::qWait(50);
    QVERIFY(animation.isRunning());
    animation.complete();
    QCOMPARE(rect.x(), qreal(200));
}

void tst_qdeclarativeanimations::resume()
{
    QDeclarative1Rectangle rect;
    QDeclarative1PropertyAnimation animation;
    animation.setTarget(&rect);
    animation.setProperty("x");
    animation.setFrom(10);
    animation.setTo(200);
    animation.setDuration(1000);
    QVERIFY(animation.from() == 10);

    animation.start();
    QTest::qWait(400);
    animation.pause();
    qreal x = rect.x();
    QVERIFY(x != qreal(200) && x != qreal(10));
    QVERIFY(animation.isRunning());
    QVERIFY(animation.isPaused());

    animation.resume();
    QVERIFY(animation.isRunning());
    QVERIFY(!animation.isPaused());
    QTest::qWait(400);
    animation.stop();
    QVERIFY(rect.x() > x);
}

void tst_qdeclarativeanimations::dotProperty()
{
    QDeclarativeEngine engine;
    QString componentStr = "import QtQuick 1.0\nRectangle{ id: rect; NumberAnimation{ objectName: \"anim\"; target: rect; property: \"border.width\"; to: 10 }}";
    QDeclarativeComponent animationComponent(&engine);
    animationComponent.setData(componentStr.toLatin1(), QUrl::fromLocalFile(""));
    QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(animationComponent.create());
    QVERIFY(rect);
    QDeclarative1NumberAnimation *animation = rect->findChild<QDeclarative1NumberAnimation*>("anim");
    QVERIFY(animation);

    animation->start();
    QTest::qWait(animation->duration()+50);
    QTIMED_COMPARE(rect->border()->width(), 10);

    rect->border()->setWidth(0);
    animation->start();
    animation->pause();
    animation->setCurrentTime(125);
    QVERIFY(animation->currentTime() == 125);
    QCOMPARE(rect->border()->width(), 5);

    delete rect;
}

void tst_qdeclarativeanimations::badTypes()
{
    //don't crash
    {
        QDeclarativeView *view = new QDeclarativeView;
        view->setSource(QUrl::fromLocalFile(SRCDIR "/data/badtype1.qml"));

        qApp->processEvents();

        delete view;
    }

    //make sure we get a compiler error
    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/badtype2.qml"));
        QTest::ignoreMessage(QtWarningMsg, "QDeclarativeComponent: Component is not ready");
        c.create();

        QVERIFY(c.errors().count() == 1);
        QCOMPARE(c.errors().at(0).description(), QLatin1String("Invalid property assignment: number expected"));
    }

    //make sure we get a compiler error
    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/badtype3.qml"));
        QTest::ignoreMessage(QtWarningMsg, "QDeclarativeComponent: Component is not ready");
        c.create();

        QVERIFY(c.errors().count() == 1);
        QCOMPARE(c.errors().at(0).description(), QLatin1String("Invalid property assignment: color expected"));
    }

    //don't crash
    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/badtype4.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarativeItemPrivate::get(rect)->setState("state1");
        QTest::qWait(1000 + 50);
        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("MyRect");
        QVERIFY(myRect);
        QCOMPARE(myRect->x(),qreal(200));
    }
}

void tst_qdeclarativeanimations::badProperties()
{
    //make sure we get a runtime error
    {
        QDeclarativeEngine engine;

        QDeclarativeComponent c1(&engine, QUrl::fromLocalFile(SRCDIR "/data/badproperty1.qml"));
        QByteArray message = QUrl::fromLocalFile(SRCDIR "/data/badproperty1.qml").toString().toUtf8() + ":18:9: QML ColorAnimation: Cannot animate non-existent property \"border.colr\"";
        QTest::ignoreMessage(QtWarningMsg, message);
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c1.create());
        QVERIFY(rect);

        QDeclarativeComponent c2(&engine, QUrl::fromLocalFile(SRCDIR "/data/badproperty2.qml"));
        message = QUrl::fromLocalFile(SRCDIR "/data/badproperty2.qml").toString().toUtf8() + ":18:9: QML ColorAnimation: Cannot animate read-only property \"border\"";
        QTest::ignoreMessage(QtWarningMsg, message);
        rect = qobject_cast<QDeclarative1Rectangle*>(c2.create());
        QVERIFY(rect);

        //### should we warn here are well?
        //rect->setState("state1");
    }
}

//test animating mixed types with property animation in a transition
//for example, int + real; color + real; etc
void tst_qdeclarativeanimations::mixedTypes()
{
    //assumes border.width stays a real -- not real robust
    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/mixedtype1.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarativeItemPrivate::get(rect)->setState("state1");
        QTest::qWait(500);
        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("MyRect");
        QVERIFY(myRect);

        //rather inexact -- is there a better way?
        QVERIFY(myRect->x() > 100 && myRect->x() < 200);
        QVERIFY(myRect->border()->width() > 1 && myRect->border()->width() < 10);
    }

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/mixedtype2.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarativeItemPrivate::get(rect)->setState("state1");
        QTest::qWait(500);
        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("MyRect");
        QVERIFY(myRect);

        //rather inexact -- is there a better way?
        QVERIFY(myRect->x() > 100 && myRect->x() < 200);
        QVERIFY(myRect->color() != QColor("red") && myRect->color() != QColor("blue"));
    }
}

void tst_qdeclarativeanimations::properties()
{
    const int waitDuration = 300;
    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/properties.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("TheRect");
        QVERIFY(myRect);
        QTest::qWait(waitDuration);
        QTIMED_COMPARE(myRect->x(),qreal(200));
    }

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/properties2.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("TheRect");
        QVERIFY(myRect);
        QTest::qWait(waitDuration);
        QTIMED_COMPARE(myRect->x(),qreal(200));
    }

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/properties3.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("TheRect");
        QVERIFY(myRect);
        QTest::qWait(waitDuration);
        QTIMED_COMPARE(myRect->x(),qreal(300));
    }

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/properties4.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("TheRect");
        QVERIFY(myRect);
        QTest::qWait(waitDuration);
        QTIMED_COMPARE(myRect->y(),qreal(200));
        QTIMED_COMPARE(myRect->x(),qreal(100));
    }

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/properties5.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("TheRect");
        QVERIFY(myRect);
        QTest::qWait(waitDuration);
        QTIMED_COMPARE(myRect->x(),qreal(100));
        QTIMED_COMPARE(myRect->y(),qreal(200));
    }
}

void tst_qdeclarativeanimations::propertiesTransition()
{
    const int waitDuration = 300;
    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/propertiesTransition.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarativeItemPrivate::get(rect)->setState("moved");
        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("TheRect");
        QVERIFY(myRect);
        QTest::qWait(waitDuration);
        QTIMED_COMPARE(myRect->x(),qreal(200));
    }

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/propertiesTransition2.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("TheRect");
        QVERIFY(myRect);
        QDeclarativeItemPrivate::get(rect)->setState("moved");
        QCOMPARE(myRect->x(),qreal(200));
        QCOMPARE(myRect->y(),qreal(100));
        QTest::qWait(waitDuration);
        QTIMED_COMPARE(myRect->y(),qreal(200));
    }

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/propertiesTransition3.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("TheRect");
        QVERIFY(myRect);
        QDeclarativeItemPrivate::get(rect)->setState("moved");
        QCOMPARE(myRect->x(),qreal(200));
        QCOMPARE(myRect->y(),qreal(100));
    }

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/propertiesTransition4.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("TheRect");
        QVERIFY(myRect);
        QDeclarativeItemPrivate::get(rect)->setState("moved");
        QCOMPARE(myRect->x(),qreal(100));
        QTest::qWait(waitDuration);
        QTIMED_COMPARE(myRect->x(),qreal(200));
    }

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/propertiesTransition5.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("TheRect");
        QVERIFY(myRect);
        QDeclarativeItemPrivate::get(rect)->setState("moved");
        QCOMPARE(myRect->x(),qreal(100));
        QTest::qWait(waitDuration);
        QTIMED_COMPARE(myRect->x(),qreal(200));
    }

    /*{
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/propertiesTransition6.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("TheRect");
        QVERIFY(myRect);
        QDeclarativeItemPrivate::get(rect)->setState("moved");
        QCOMPARE(myRect->x(),qreal(100));
        QTest::qWait(waitDuration);
        QTIMED_COMPARE(myRect->x(),qreal(100));
    }*/

    {
        QDeclarativeEngine engine;
        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/propertiesTransition7.qml"));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarativeItemPrivate::get(rect)->setState("moved");
        QDeclarative1Rectangle *myRect = rect->findChild<QDeclarative1Rectangle*>("TheRect");
        QVERIFY(myRect);
        QTest::qWait(waitDuration);
        QTIMED_COMPARE(myRect->x(),qreal(200));
    }

}

void tst_qdeclarativeanimations::invalidDuration()
{
    QDeclarative1PropertyAnimation *animation = new QDeclarative1PropertyAnimation;
    QTest::ignoreMessage(QtWarningMsg, "<Unknown File>: QML PropertyAnimation: Cannot set a duration of < 0");
    animation->setDuration(-1);
    QCOMPARE(animation->duration(), 250);

    QDeclarative1PauseAnimation *pauseAnimation = new QDeclarative1PauseAnimation;
    QTest::ignoreMessage(QtWarningMsg, "<Unknown File>: QML PauseAnimation: Cannot set a duration of < 0");
    pauseAnimation->setDuration(-1);
    QCOMPARE(pauseAnimation->duration(), 250);
}

void tst_qdeclarativeanimations::attached()
{
    QDeclarativeEngine engine;

    QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/attached.qml"));
    QTest::ignoreMessage(QtDebugMsg, "off");
    QTest::ignoreMessage(QtDebugMsg, "on");
    QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
    QVERIFY(rect);
}

void tst_qdeclarativeanimations::propertyValueSourceDefaultStart()
{
    {
        QDeclarativeEngine engine;

        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/valuesource.qml"));

        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1AbstractAnimation *myAnim = rect->findChild<QDeclarative1AbstractAnimation*>("MyAnim");
        QVERIFY(myAnim);
        QVERIFY(myAnim->isRunning());
    }

    {
        QDeclarativeEngine engine;

        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/valuesource2.qml"));

        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1AbstractAnimation *myAnim = rect->findChild<QDeclarative1AbstractAnimation*>("MyAnim");
        QVERIFY(myAnim);
        QVERIFY(myAnim->isRunning() == false);
    }

    {
        QDeclarativeEngine engine;

        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/dontAutoStart.qml"));

        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1AbstractAnimation *myAnim = rect->findChild<QDeclarative1AbstractAnimation*>("MyAnim");
        QVERIFY(myAnim && myAnim->qtAnimation());
        QVERIFY(myAnim->qtAnimation()->state() == QAbstractAnimation::Stopped);
    }
}


void tst_qdeclarativeanimations::dontStart()
{
    {
        QDeclarativeEngine engine;

        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/dontStart.qml"));

        QString warning = c.url().toString() + ":14:13: QML NumberAnimation: setRunning() cannot be used on non-root animation nodes.";
        QTest::ignoreMessage(QtWarningMsg, qPrintable(warning));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1AbstractAnimation *myAnim = rect->findChild<QDeclarative1AbstractAnimation*>("MyAnim");
        QVERIFY(myAnim && myAnim->qtAnimation());
        QVERIFY(myAnim->qtAnimation()->state() == QAbstractAnimation::Stopped);
    }

    {
        QDeclarativeEngine engine;

        QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/dontStart2.qml"));

        QString warning = c.url().toString() + ":15:17: QML NumberAnimation: setRunning() cannot be used on non-root animation nodes.";
        QTest::ignoreMessage(QtWarningMsg, qPrintable(warning));
        QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
        QVERIFY(rect);

        QDeclarative1AbstractAnimation *myAnim = rect->findChild<QDeclarative1AbstractAnimation*>("MyAnim");
        QVERIFY(myAnim && myAnim->qtAnimation());
        QVERIFY(myAnim->qtAnimation()->state() == QAbstractAnimation::Stopped);
    }
}

void tst_qdeclarativeanimations::easingProperties()
{
    {
        QDeclarativeEngine engine;
        QString componentStr = "import QtQuick 1.0\nNumberAnimation { easing.type: \"InOutQuad\" }";
        QDeclarativeComponent animationComponent(&engine);
        animationComponent.setData(componentStr.toLatin1(), QUrl::fromLocalFile(""));
        QDeclarative1PropertyAnimation *animObject = qobject_cast<QDeclarative1PropertyAnimation*>(animationComponent.create());

        QVERIFY(animObject != 0);
        QCOMPARE(animObject->easing().type(), QEasingCurve::InOutQuad);
    }

    {
        QDeclarativeEngine engine;
        QString componentStr = "import QtQuick 1.0\nPropertyAnimation { easing.type: \"OutBounce\"; easing.amplitude: 5.0 }";
        QDeclarativeComponent animationComponent(&engine);
        animationComponent.setData(componentStr.toLatin1(), QUrl::fromLocalFile(""));
        QDeclarative1PropertyAnimation *animObject = qobject_cast<QDeclarative1PropertyAnimation*>(animationComponent.create());

        QVERIFY(animObject != 0);
        QCOMPARE(animObject->easing().type(), QEasingCurve::OutBounce);
        QCOMPARE(animObject->easing().amplitude(), 5.0);
    }

    {
        QDeclarativeEngine engine;
        QString componentStr = "import QtQuick 1.0\nPropertyAnimation { easing.type: \"OutElastic\"; easing.amplitude: 5.0; easing.period: 3.0}";
        QDeclarativeComponent animationComponent(&engine);
        animationComponent.setData(componentStr.toLatin1(), QUrl::fromLocalFile(""));
        QDeclarative1PropertyAnimation *animObject = qobject_cast<QDeclarative1PropertyAnimation*>(animationComponent.create());

        QVERIFY(animObject != 0);
        QCOMPARE(animObject->easing().type(), QEasingCurve::OutElastic);
        QCOMPARE(animObject->easing().amplitude(), 5.0);
        QCOMPARE(animObject->easing().period(), 3.0);
    }

    {
        QDeclarativeEngine engine;
        QString componentStr = "import QtQuick 1.0\nPropertyAnimation { easing.type: \"InOutBack\"; easing.overshoot: 2 }";
        QDeclarativeComponent animationComponent(&engine);
        animationComponent.setData(componentStr.toLatin1(), QUrl::fromLocalFile(""));
        QDeclarative1PropertyAnimation *animObject = qobject_cast<QDeclarative1PropertyAnimation*>(animationComponent.create());

        QVERIFY(animObject != 0);
        QCOMPARE(animObject->easing().type(), QEasingCurve::InOutBack);
        QCOMPARE(animObject->easing().overshoot(), 2.0);
    }
}

void tst_qdeclarativeanimations::rotation()
{
    QDeclarativeEngine engine;
    QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/rotation.qml"));
    QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
    QVERIFY(rect);

    QDeclarative1Rectangle *rr = rect->findChild<QDeclarative1Rectangle*>("rr");
    QDeclarative1Rectangle *rr2 = rect->findChild<QDeclarative1Rectangle*>("rr2");
    QDeclarative1Rectangle *rr3 = rect->findChild<QDeclarative1Rectangle*>("rr3");
    QDeclarative1Rectangle *rr4 = rect->findChild<QDeclarative1Rectangle*>("rr4");

    QDeclarativeItemPrivate::get(rect)->setState("state1");
    QTest::qWait(800);
    qreal r1 = rr->rotation();
    qreal r2 = rr2->rotation();
    qreal r3 = rr3->rotation();
    qreal r4 = rr4->rotation();

    QVERIFY(r1 > qreal(0) && r1 < qreal(370));
    QVERIFY(r2 > qreal(0) && r2 < qreal(370));
    QVERIFY(r3 < qreal(0) && r3 > qreal(-350));
    QVERIFY(r4 > qreal(0) && r4 < qreal(10));
    QCOMPARE(r1,r2);
    QVERIFY(r4 < r2);

    QTest::qWait(800);
    QTIMED_COMPARE(rr->rotation() + rr2->rotation() + rr3->rotation() + rr4->rotation(), qreal(370*4));
}

void tst_qdeclarativeanimations::runningTrueBug()
{
    //ensure we start correctly when "running: true" is explicitly set
    QDeclarativeEngine engine;
    QDeclarativeComponent c(&engine, QUrl::fromLocalFile(SRCDIR "/data/runningTrueBug.qml"));
    QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
    QVERIFY(rect);

    QDeclarative1Rectangle *cloud = rect->findChild<QDeclarative1Rectangle*>("cloud");
    QVERIFY(cloud);
    QTest::qWait(1000);
    QVERIFY(cloud->x() > qreal(0));
}

//QTBUG-12805
void tst_qdeclarativeanimations::nonTransitionBug()
{
    //tests that the animation values from the previous transition are properly cleared
    //in the case where an animation in the transition doesn't match anything (but previously did)
    QDeclarativeEngine engine;

    QDeclarativeComponent c(&engine, SRCDIR "/data/nonTransitionBug.qml");
    QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
    QVERIFY(rect != 0);
    QDeclarativeItemPrivate *rectPrivate = QDeclarativeItemPrivate::get(rect);
    QDeclarative1Rectangle *mover = rect->findChild<QDeclarative1Rectangle*>("mover");

    mover->setX(100);
    QCOMPARE(mover->x(), qreal(100));

    rectPrivate->setState("left");
    QTRY_COMPARE(mover->x(), qreal(0));

    mover->setX(100);
    QCOMPARE(mover->x(), qreal(100));

    //make sure we don't try to animate back to 0
    rectPrivate->setState("free");
    QTest::qWait(300);
    QCOMPARE(mover->x(), qreal(100));
}

//QTBUG-14042
void tst_qdeclarativeanimations::registrationBug()
{
    QDeclarativeEngine engine;

    QDeclarativeComponent c(&engine, SRCDIR "/data/registrationBug.qml");
    QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
    QVERIFY(rect != 0);
    QTRY_COMPARE(rect->property("value"), QVariant(int(100)));
}

void tst_qdeclarativeanimations::doubleRegistrationBug()
{
    QDeclarativeEngine engine;

    QDeclarativeComponent c(&engine, SRCDIR "/data/doubleRegistrationBug.qml");
    QDeclarative1Rectangle *rect = qobject_cast<QDeclarative1Rectangle*>(c.create());
    QVERIFY(rect != 0);

    QDeclarative1AbstractAnimation *anim = rect->findChild<QDeclarative1AbstractAnimation*>("animation");
    QVERIFY(anim != 0);
    QTRY_COMPARE(anim->qtAnimation()->state(), QAbstractAnimation::Stopped);
}

//QTBUG-16736
void tst_qdeclarativeanimations::alwaysRunToEndRestartBug()
{
    QDeclarative1Rectangle rect;
    QDeclarative1PropertyAnimation animation;
    animation.setTarget(&rect);
    animation.setProperty("x");
    animation.setTo(200);
    animation.setDuration(1000);
    animation.setLoops(-1);
    animation.setAlwaysRunToEnd(true);
    QVERIFY(animation.loops() == -1);
    QVERIFY(animation.alwaysRunToEnd() == true);
    animation.start();
    animation.stop();
    animation.start();
    animation.stop();
    QTest::qWait(500);
    QVERIFY(rect.x() != qreal(200));
    QTest::qWait(800);
    QTIMED_COMPARE(rect.x(), qreal(200));
    QCOMPARE(static_cast<QDeclarative1AbstractAnimation*>(&animation)->qtAnimation()->state(), QAbstractAnimation::Stopped);
}

QTEST_MAIN(tst_qdeclarativeanimations)

#include "tst_qdeclarativeanimations.moc"
