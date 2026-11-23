/****************************************************************************
** Meta object code from reading C++ file 'ChecklistModel.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/models/ChecklistModel.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ChecklistModel.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN7cadence14ChecklistModelE_t {};
} // unnamed namespace

template <> constexpr inline auto cadence::ChecklistModel::qt_create_metaobjectdata<qt_meta_tag_ZN7cadence14ChecklistModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "cadence::ChecklistModel",
        "countChanged",
        "",
        "reload",
        "addChecklist",
        "title",
        "subjectId",
        "removeChecklist",
        "checklistId",
        "count",
        "Roles",
        "IdRole",
        "SubjectIdRole",
        "TitleRole",
        "CompletedCountRole",
        "TotalCountRole"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'countChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'reload'
        QtMocHelpers::MethodData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'addChecklist'
        QtMocHelpers::MethodData<bool(const QString &, qint64)>(4, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 5 }, { QMetaType::LongLong, 6 },
        }}),
        // Method 'addChecklist'
        QtMocHelpers::MethodData<bool(const QString &)>(4, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Bool, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'removeChecklist'
        QtMocHelpers::MethodData<bool(qint64)>(7, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::LongLong, 8 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'count'
        QtMocHelpers::PropertyData<int>(9, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'Roles'
        QtMocHelpers::EnumData<enum Roles>(10, 10, QMC::EnumFlags{}).add({
            {   11, Roles::IdRole },
            {   12, Roles::SubjectIdRole },
            {   13, Roles::TitleRole },
            {   14, Roles::CompletedCountRole },
            {   15, Roles::TotalCountRole },
        }),
    };
    return QtMocHelpers::metaObjectData<ChecklistModel, qt_meta_tag_ZN7cadence14ChecklistModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject cadence::ChecklistModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence14ChecklistModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence14ChecklistModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7cadence14ChecklistModelE_t>.metaTypes,
    nullptr
} };

void cadence::ChecklistModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ChecklistModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->countChanged(); break;
        case 1: _t->reload(); break;
        case 2: { bool _r = _t->addChecklist((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 3: { bool _r = _t->addChecklist((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->removeChecklist((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ChecklistModel::*)()>(_a, &ChecklistModel::countChanged, 0))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->rowCount(); break;
        default: break;
        }
    }
}

const QMetaObject *cadence::ChecklistModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cadence::ChecklistModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence14ChecklistModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int cadence::ChecklistModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void cadence::ChecklistModel::countChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
namespace {
struct qt_meta_tag_ZN7cadence18ChecklistItemModelE_t {};
} // unnamed namespace

template <> constexpr inline auto cadence::ChecklistItemModel::qt_create_metaobjectdata<qt_meta_tag_ZN7cadence18ChecklistItemModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "cadence::ChecklistItemModel",
        "activeChecklistIdChanged",
        "",
        "reload",
        "addItem",
        "title",
        "parentItemId",
        "toggleItem",
        "itemId",
        "removeItem",
        "activeChecklistId",
        "Roles",
        "IdRole",
        "ParentItemIdRole",
        "TitleRole",
        "IsCompletedRole",
        "DepthRole",
        "HasChildrenRole"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'activeChecklistIdChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'reload'
        QtMocHelpers::MethodData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'addItem'
        QtMocHelpers::MethodData<bool(const QString &, qint64)>(4, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 5 }, { QMetaType::LongLong, 6 },
        }}),
        // Method 'addItem'
        QtMocHelpers::MethodData<bool(const QString &)>(4, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Bool, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'toggleItem'
        QtMocHelpers::MethodData<bool(qint64)>(7, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::LongLong, 8 },
        }}),
        // Method 'removeItem'
        QtMocHelpers::MethodData<bool(qint64)>(9, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::LongLong, 8 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'activeChecklistId'
        QtMocHelpers::PropertyData<qint64>(10, QMetaType::LongLong, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 0),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'Roles'
        QtMocHelpers::EnumData<enum Roles>(11, 11, QMC::EnumFlags{}).add({
            {   12, Roles::IdRole },
            {   13, Roles::ParentItemIdRole },
            {   14, Roles::TitleRole },
            {   15, Roles::IsCompletedRole },
            {   16, Roles::DepthRole },
            {   17, Roles::HasChildrenRole },
        }),
    };
    return QtMocHelpers::metaObjectData<ChecklistItemModel, qt_meta_tag_ZN7cadence18ChecklistItemModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject cadence::ChecklistItemModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence18ChecklistItemModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence18ChecklistItemModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7cadence18ChecklistItemModelE_t>.metaTypes,
    nullptr
} };

void cadence::ChecklistItemModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ChecklistItemModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->activeChecklistIdChanged(); break;
        case 1: _t->reload(); break;
        case 2: { bool _r = _t->addItem((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[2])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 3: { bool _r = _t->addItem((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->toggleItem((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->removeItem((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ChecklistItemModel::*)()>(_a, &ChecklistItemModel::activeChecklistIdChanged, 0))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<qint64*>(_v) = _t->activeChecklistId(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setActiveChecklistId(*reinterpret_cast<qint64*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *cadence::ChecklistItemModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cadence::ChecklistItemModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence18ChecklistItemModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int cadence::ChecklistItemModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void cadence::ChecklistItemModel::activeChecklistIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
