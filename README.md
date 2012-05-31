QxClipper
=========

[Clipper](http://www.angusj.com/delphi/clipper.php) wrapper class for Qt.

How to use
----------

1. Run and build QxClipper.pro.
2. Include QxClipper.pri to your project file.

Repository
----------

You can get the current development version from the [git repository](https://github.com/Skyrpex/QxClipper).

About Clipper
-------

The Clipper library primarily performs the boolean clipping operations - intersection, union, difference & xor - on 2D polygons. It also performs polygon offsetting.
The library handles complex (self-intersecting) polygons, polygons with holes and polygons with overlapping co-linear edges.
Input polygons for clipping can use EvenOdd, NonZero, Positive and Negative filling modes.
The clipping code is based on the [Vatti clipping algorithm](http://en.wikipedia.org/wiki/Vatti_clipping_algorithm), and [out performs other clipping libraries](http://www.angusj.com/delphi/clipper.php#features).