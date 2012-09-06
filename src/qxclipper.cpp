#include <QxClipper>
#include <QDebug>
#include "clipper/clipper.hpp"

static const int ConversionFactor = 100;

Qx::ClipType clipTypeFromClipper(ClipperLib::ClipType type)
{
  switch(type)
  {
  case ClipperLib::ctDifference:
    return Qx::Difference;
  case ClipperLib::ctIntersection:
    return Qx::Intersection;
  case ClipperLib::ctUnion:
    return Qx::Union;
  case ClipperLib::ctXor:
    return Qx::Xor;
  default:
    return static_cast<Qx::ClipType>(type);
  }
}

ClipperLib::ClipType clipTypeFromQxClipper(Qx::ClipType type)
{
  switch(type)
  {
  case Qx::Difference:
    return ClipperLib::ctDifference;
  case Qx::Intersection:
    return ClipperLib::ctIntersection;
  case Qx::Union:
    return ClipperLib::ctUnion;
  case Qx::Xor:
    return ClipperLib::ctXor;
  default:
    return static_cast<ClipperLib::ClipType>(type);
  }
}

Qt::PenJoinStyle joinTypeFromClipper(ClipperLib::JoinType type)
{
  switch(type)
  {
  case ClipperLib::jtMiter:
    return Qt::MiterJoin;
  case ClipperLib::jtRound:
    return Qt::RoundJoin;
  case ClipperLib::jtSquare:
    return Qt::BevelJoin;
  default:
    return Qt::BevelJoin;
  }
}

ClipperLib::JoinType joinTypeFromQxClipper(Qt::PenJoinStyle type)
{
  switch(type)
  {
  case Qt::MiterJoin:
    return ClipperLib::jtMiter;
  case Qt::RoundJoin:
    return ClipperLib::jtRound;
  case Qt::BevelJoin:
    return ClipperLib::jtSquare;
  default:
    return ClipperLib::jtSquare;
  }
}

Qx::Orientation orientationFromClipper(bool orientation)
{
  if(orientation)
    return Qx::Clockwise;
  return Qx::CounterClockwise;
}

bool orientationFromQxClipper(Qx::Orientation type)
{
  return (type == Qx::Clockwise);
}

void polygonFromQxClipper(const QPolygonF& qxPolygon,
                          ClipperLib::Polygon &clipperPolygon)
{
  clipperPolygon.clear();
  clipperPolygon.reserve(qxPolygon.size());
  foreach(const QPointF &point, qxPolygon)
  {
    clipperPolygon.push_back(ClipperLib::IntPoint(point.x()*ConversionFactor,
                                                  point.y()*ConversionFactor));
  }
}

void polygonsFromQxClipper(const QList<QPolygonF>& qxPolygons,
                           ClipperLib::Polygons &clipperPolygons)
{
  clipperPolygons.resize(qxPolygons.count());
  for(unsigned int i = 0; i < clipperPolygons.size(); ++i)
    polygonFromQxClipper(qxPolygons[i], clipperPolygons[i]);
}

void polygonFromClipper(const ClipperLib::Polygon &clipperPolygon,
                        QPolygonF &qxPolygon)
{
  qxPolygon.clear();
  qxPolygon.resize(clipperPolygon.size());
  for(unsigned int i = 0; i < clipperPolygon.size(); ++i)
  {
    qxPolygon[i] = QPointF(static_cast<qreal>(clipperPolygon[i].X)/ConversionFactor,
                           static_cast<qreal>(clipperPolygon[i].Y)/ConversionFactor);
  }
}

void polygonsFromClipper(const ClipperLib::Polygons &clipperPolygons,
                         QList<QPolygonF> &result)
{
  // Return
  result.clear();
  result.reserve(clipperPolygons.size());
  for(unsigned int i = 0; i < clipperPolygons.size(); ++i)
  {
    QPolygonF poly;
    polygonFromClipper(clipperPolygons[i], poly);
    result << poly;
  }
}

void setOrientationHelper(ClipperLib::Polygon &polygon, bool orientation)
{
  if(ClipperLib::Orientation(polygon) != orientation)
    ClipperLib::ReversePolygon(polygon);
}




QList<QPolygonF> QxClipper::merged(const QList<QPolygonF> &polygons)
{
  ClipperLib::Polygons clipperPolygons;
  polygonsFromQxClipper(polygons, clipperPolygons);

  ClipperLib::Clipper clipper;
  clipper.AddPolygons(clipperPolygons, ClipperLib::ptSubject);

  ClipperLib::Polygons solution;
  clipper.Execute(ClipperLib::ctUnion, solution, ClipperLib::pftNonZero);

  // Return
  QList<QPolygonF> result;
  polygonsFromClipper(solution, result);
  return result;
}

//bool QxClipper::addPolygon(const QPolygonF &polygon, Qx::PolygonType type)
//{
//  ClipperLib::Polygon clipperPolygon;
//  polygonFromQxClipper(polygon, clipperPolygon);
//  return m_clipper.AddPolygon(clipperPolygon, polygonFromQxClipper(type));
//}

Qx::Orientation QxClipper::orientation(const QPolygonF &polygon)
{
  ClipperLib::Polygon clipperPolygon;
  polygonFromQxClipper(polygon, clipperPolygon);
  return orientationFromClipper(ClipperLib::Orientation(clipperPolygon));
}

void QxClipper::setOrientation(QPolygonF &polygon, Qx::Orientation orientation)
{
  ClipperLib::Polygon clipperPolygon;
  polygonFromQxClipper(polygon, clipperPolygon);
  setOrientationHelper(clipperPolygon, orientationFromQxClipper(orientation));
  polygonFromClipper(clipperPolygon, polygon);
}

/**
  */
qreal QxClipper::area(const QPolygonF &polygon)
{
  ClipperLib::Polygon poly;
  polygonFromQxClipper(polygon, poly);
  return static_cast<qreal>(ClipperLib::Area(poly)/(ConversionFactor*ConversionFactor));
}

/**
  */
QPolygonF QxClipper::offseted(const QPolygonF &polygon,
                              qreal delta,
                              Qt::PenJoinStyle joinStyle,
                              qreal miterLimit)
{
  ClipperLib::Polygons polygons(1);
  polygonFromQxClipper(polygon, polygons[0]);
  ClipperLib::OffsetPolygons(polygons, polygons,
                             static_cast<double>(delta*ConversionFactor),
                             joinTypeFromQxClipper(joinStyle),
                             static_cast<double>(miterLimit));

  if(polygon.isEmpty())
    return QPolygonF();
  QPolygonF result;
  polygonFromClipper(polygons.front(), result);
  return result;
}

/**
  */
QList<QPolygonF> QxClipper::offseted(const QList<QPolygonF> &polygons,
                                     qreal delta,
                                     Qt::PenJoinStyle joinStyle,
                                     qreal miterLimit)
{
  // Offset
  ClipperLib::Polygons clipperPolygons;
  polygonsFromQxClipper(polygons, clipperPolygons);
  ClipperLib::OffsetPolygons(clipperPolygons, clipperPolygons,
                             static_cast<double>(delta*ConversionFactor),
                             joinTypeFromQxClipper(joinStyle),
                             static_cast<double>(miterLimit));

  // Return
  QList<QPolygonF> result;
  polygonsFromClipper(clipperPolygons, result);
  return result;
}

/**
  */
QList<QPolygonF> QxClipper::simplified(const QPolygonF &polygon)
{
  ClipperLib::Polygon clipperPolygon;
  polygonFromQxClipper(polygon, clipperPolygon);

  ClipperLib::Polygons clipperPolygons;
  ClipperLib::SimplifyPolygon(clipperPolygon,
                              clipperPolygons);

  // Return
  QList<QPolygonF> result;
  polygonsFromClipper(clipperPolygons, result);
  return result;
}

/**
  */
QList<QPolygonF> QxClipper::simplified(const QList<QPolygonF> &polygons)
{
  ClipperLib::Polygons clipperPolygons;
  polygonsFromQxClipper(polygons, clipperPolygons);

  ClipperLib::SimplifyPolygons(clipperPolygons);

  // Return
  QList<QPolygonF> result;
  polygonsFromClipper(clipperPolygons, result);
  return result;
}

/**
  */
QPolygonF QxClipper::reversed(const QPolygonF &polygon)
{
  ClipperLib::Polygon clipperPolygon;
  polygonFromQxClipper(polygon, clipperPolygon);
  ClipperLib::ReversePolygon(clipperPolygon);

  QPolygonF result;
  polygonFromClipper(clipperPolygon, result);
  return result;
}

/**
  */
QList<QPolygonF> reversed(const QList<QPolygonF> &polygons)
{
  ClipperLib::Polygons clipperPolygons(polygons.count());
  for(unsigned int i = 0; i < clipperPolygons.size(); ++i)
    polygonFromQxClipper(polygons[i], clipperPolygons[i]);
  ClipperLib::ReversePolygons(clipperPolygons);

  // Return
  QList<QPolygonF> result;
  result.reserve(clipperPolygons.size());
  for(unsigned int i = 0; i < clipperPolygons.size(); ++i)
  {
    QPolygonF poly;
    polygonFromClipper(clipperPolygons[i], poly);
    result << poly;
  }
  return result;
}

/**
  */
bool QxClipper::isComplex(const QPolygonF &polygon)
{
  ClipperLib::Polygon clipperPolygon;
  polygonFromQxClipper(polygon, clipperPolygon);

  ClipperLib::Polygons simplified;
  ClipperLib::SimplifyPolygon(clipperPolygon, simplified);
  return simplified.size() > 1;
}
