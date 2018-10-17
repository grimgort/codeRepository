/****************************************************************************
** Meta object code from reading C++ file 'mafenetre.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QT1/mafenetre.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mafenetre.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MaFenetre2_t {
    QByteArrayData data[7];
    char stringdata0[76];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MaFenetre2_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MaFenetre2_t qt_meta_stringdata_MaFenetre2 = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MaFenetre2"
QT_MOC_LITERAL(1, 11, 17), // "agrandissementMax"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 14), // "changerLargeur"
QT_MOC_LITERAL(4, 45, 7), // "largeur"
QT_MOC_LITERAL(5, 53, 14), // "changerHauteur"
QT_MOC_LITERAL(6, 68, 7) // "hauteur"

    },
    "MaFenetre2\0agrandissementMax\0\0"
    "changerLargeur\0largeur\0changerHauteur\0"
    "hauteur"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MaFenetre2[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   30,    2, 0x0a /* Public */,
       5,    1,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void MaFenetre2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MaFenetre2 *_t = static_cast<MaFenetre2 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->agrandissementMax(); break;
        case 1: _t->changerLargeur((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->changerHauteur((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MaFenetre2::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MaFenetre2::agrandissementMax)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MaFenetre2::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MaFenetre2.data,
      qt_meta_data_MaFenetre2,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MaFenetre2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MaFenetre2::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MaFenetre2.stringdata0))
        return static_cast<void*>(const_cast< MaFenetre2*>(this));
    return QWidget::qt_metacast(_clname);
}

int MaFenetre2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MaFenetre2::agrandissementMax()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
struct qt_meta_stringdata_MaFenetre3_t {
    QByteArrayData data[3];
    char stringdata0[27];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MaFenetre3_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MaFenetre3_t qt_meta_stringdata_MaFenetre3 = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MaFenetre3"
QT_MOC_LITERAL(1, 11, 14), // "ouvrirDialogue"
QT_MOC_LITERAL(2, 26, 0) // ""

    },
    "MaFenetre3\0ouvrirDialogue\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MaFenetre3[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void MaFenetre3::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MaFenetre3 *_t = static_cast<MaFenetre3 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ouvrirDialogue(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MaFenetre3::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MaFenetre3.data,
      qt_meta_data_MaFenetre3,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MaFenetre3::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MaFenetre3::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MaFenetre3.stringdata0))
        return static_cast<void*>(const_cast< MaFenetre3*>(this));
    return QWidget::qt_metacast(_clname);
}

int MaFenetre3::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_MaFenetre4_t {
    QByteArrayData data[3];
    char stringdata0[27];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MaFenetre4_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MaFenetre4_t qt_meta_stringdata_MaFenetre4 = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MaFenetre4"
QT_MOC_LITERAL(1, 11, 14), // "ouvrirDialogue"
QT_MOC_LITERAL(2, 26, 0) // ""

    },
    "MaFenetre4\0ouvrirDialogue\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MaFenetre4[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void MaFenetre4::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MaFenetre4 *_t = static_cast<MaFenetre4 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ouvrirDialogue(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MaFenetre4::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MaFenetre4.data,
      qt_meta_data_MaFenetre4,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MaFenetre4::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MaFenetre4::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MaFenetre4.stringdata0))
        return static_cast<void*>(const_cast< MaFenetre4*>(this));
    return QWidget::qt_metacast(_clname);
}

int MaFenetre4::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_MaFenetre5_t {
    QByteArrayData data[4];
    char stringdata0[41];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MaFenetre5_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MaFenetre5_t qt_meta_stringdata_MaFenetre5 = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MaFenetre5"
QT_MOC_LITERAL(1, 11, 14), // "ouvrirDialogue"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 13) // "ouvrirDossier"

    },
    "MaFenetre5\0ouvrirDialogue\0\0ouvrirDossier"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MaFenetre5[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    0,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MaFenetre5::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MaFenetre5 *_t = static_cast<MaFenetre5 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ouvrirDialogue(); break;
        case 1: _t->ouvrirDossier(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MaFenetre5::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MaFenetre5.data,
      qt_meta_data_MaFenetre5,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MaFenetre5::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MaFenetre5::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MaFenetre5.stringdata0))
        return static_cast<void*>(const_cast< MaFenetre5*>(this));
    return QWidget::qt_metacast(_clname);
}

int MaFenetre5::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
