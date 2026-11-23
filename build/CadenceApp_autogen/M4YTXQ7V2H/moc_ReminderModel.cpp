/****************************************************************************
** Meta object code from reading C++ file 'ReminderModel.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/models/ReminderModel.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ReminderModel.hpp' doesn't include <QObject>."
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
struct qt_meta_tag_ZN7cadence13ReminderModelE_t {};
} // unnamed namespace

template <> constexpr inline auto cadence::ReminderModel::qt_create_metaobjectdata<qt_meta_tag_ZN7cadence13ReminderModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "cadence::ReminderModel",
        "countChanged",
        "",
        "reminderScheduleChanged",
        "reload",
        "addReminder",
        "title",
        "description",
        "categoryStr",
        "subjectId",
        "dueAtEpoch",
        "dueDateIso",
        "toggleCompleted",
        "id",
        "pushToTomorrow",
        "removeReminder",
        "count",
        "Roles",
        "IdRole",
        "SubjectIdRole",
        "SubjectNameRole",
        "SubjectColorHexRole",
        "TitleRole",
        "DescriptionRole",
        "CategoryRole",
        "DueAtRole",
        "DueDateRole",
        "IsCompletedRole"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'countChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'reminderScheduleChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'reload'
        QtMocHelpers::MethodData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'addReminder'
        QtMocHelpers::MethodData<bool(const QString &, const QString &, const QString &, qint64, qint64, const QString &)>(5, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 6 }, { QMetaType::QString, 7 }, { QMetaType::QString, 8 }, { QMetaType::LongLong, 9 },
            { QMetaType::LongLong, 10 }, { QMetaType::QString, 11 },
        }}),
        // Method 'toggleCompleted'
        QtMocHelpers::MethodData<bool(qint64)>(12, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::LongLong, 13 },
        }}),
        // Method 'pushToTomorrow'
        QtMocHelpers::MethodData<bool(qint64)>(14, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::LongLong, 13 },
        }}),
        // Method 'removeReminder'
        QtMocHelpers::MethodData<bool(qint64)>(15, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::LongLong, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'count'
        QtMocHelpers::PropertyData<int>(16, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'Roles'
        QtMocHelpers::EnumData<enum Roles>(17, 17, QMC::EnumFlags{}).add({
            {   18, Roles::IdRole },
            {   19, Roles::SubjectIdRole },
            {   20, Roles::SubjectNameRole },
            {   21, Roles::SubjectColorHexRole },
            {   22, Roles::TitleRole },
            {   23, Roles::DescriptionRole },
            {   24, Roles::CategoryRole },
            {   25, Roles::DueAtRole },
            {   26, Roles::DueDateRole },
            {   27, Roles::IsCompletedRole },
        }),
    };
    return QtMocHelpers::metaObjectData<ReminderModel, qt_meta_tag_ZN7cadence13ReminderModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject cadence::ReminderModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence13ReminderModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence13ReminderModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7cadence13ReminderModelE_t>.metaTypes,
    nullptr
} };

void cadence::ReminderModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ReminderModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->countChanged(); break;
        case 1: _t->reminderScheduleChanged(); break;
        case 2: _t->reload(); break;
        case 3: { bool _r = _t->addReminder((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[6])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->toggleCompleted((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->pushToTomorrow((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        case 6: { bool _r = _t->removeReminder((*reinterpret_cast<std::add_pointer_t<qint64>>(_a[1])));
            if (_a[0]) *reinterpret_cast<bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ReminderModel::*)()>(_a, &ReminderModel::countChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ReminderModel::*)()>(_a, &ReminderModel::reminderScheduleChanged, 1))
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

const QMetaObject *cadence::ReminderModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *cadence::ReminderModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7cadence13ReminderModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int cadence::ReminderModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
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
void cadence::ReminderModel::countChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void cadence::ReminderModel::reminderScheduleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
