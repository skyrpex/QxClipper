#ifndef QXCLIPPER_H
#define QXCLIPPER_H

#include <qxclipper_global.h>
#include <QPolygonF>
#include <QList>

namespace Qx
{
/**
  */
enum Orientation
{
  Clockwise,
  CounterClockwise
};

/**
  */
enum PolygonType
{
  Subject,
  Clip
};

/**
  */
enum ClipType
{
  Intersection,
  Union,
  Difference,
  Xor
};

/**
  */
enum FillRule
{
  OddEvenFill,
  NonZeroFill,
  PositiveFill,
  NegativeFill
};
}

//class ClipperLib::Clipper;

/**
  */
class QXCLIPPER_EXPORT QxClipper
{
public:
  struct Polygon
  {
    QPolygonF boundary;
    QList<QPolygonF> holes;
  };

//  bool addPolygon(const QPolygonF &polygon, Qx::PolygonType type);
//  bool execute();

  static QList<QPolygonF> merged(const QList<QPolygonF> &polygons);

  /**
    */
  static Qx::Orientation orientation(const QPolygonF &polygon);

  /**
    */
  static void setOrientation(QPolygonF &polygon,
                             Qx::Orientation orientation);

  /**
    */
  static qreal area(const QPolygonF &polygon);

  /**
    */
  static QPolygonF offseted(const QPolygonF &polygon,
                            qreal delta,
                            Qt::PenJoinStyle joinStyle = Qt::BevelJoin,
                            qreal miterLimit = 2.0);

  /**
    */
  static QList<QPolygonF> offseted(const QList<QPolygonF> &polygons,
                                   qreal delta,
                                   Qt::PenJoinStyle joinStyle = Qt::BevelJoin,
                                   qreal miterLimit = 2.0);

  /**
    */
  static QList<QPolygonF> simplified(const QPolygonF &polygon);

  /**
    */
  static QList<QPolygonF> simplified(const QList<QPolygonF> &polygons);

  /**
    */
  static QPolygonF reversed(const QPolygonF &polygon);

  /**
    */
  static QList<QPolygonF> reversed(const QList<QPolygonF> &polygons);

  /**
    */
  static bool isComplex(const QPolygonF &polygon);

private:
//  ClipperLib::Clipper m_clipper;
};

#endif // QXCLIPPER_H
