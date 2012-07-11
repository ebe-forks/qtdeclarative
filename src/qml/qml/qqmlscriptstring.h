/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtQml module of the Qt Toolkit.
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QQMLSCRIPTSTRING_H
#define QQMLSCRIPTSTRING_H

#include <QtQml/qtqmlglobal.h>
#include <QtCore/qstring.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE


class QObject;
class QQmlContext;
class QQmlScriptStringPrivate;
class Q_QML_EXPORT QQmlScriptString 
{
public:
    QQmlScriptString();
    QQmlScriptString(const QQmlScriptString &);
    ~QQmlScriptString();

    QQmlScriptString &operator=(const QQmlScriptString &);

    bool isEmpty() const;

    bool isUndefinedLiteral() const;
    bool isNullLiteral() const;
    QString stringLiteral() const;
    qreal numberLiteral(bool *ok) const;
    bool booleanLiteral(bool *ok) const;

private:
    QQmlScriptString(const QString &script, QQmlContext *context, QObject *scope);
    QSharedDataPointer<QQmlScriptStringPrivate> d;

    friend class QQmlScriptStringPrivate;
    friend class QQmlVME;
    friend class QQmlExpression;
    friend class QQmlBinding;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QQmlScriptString)

QT_END_HEADER

#endif // QQMLSCRIPTSTRING_H

