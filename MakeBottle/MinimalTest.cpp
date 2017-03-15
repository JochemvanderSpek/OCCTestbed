
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>

#include <BRepLib.hxx>

#include <BRepOffsetAPI_ThruSections.hxx>


#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>

#include <gp.hxx>
#include <gp_Pnt.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>

TopoDS_Shape
MakeBottle(const Standard_Real myWidth, const Standard_Real myHeight,
           const Standard_Real myThickness)
{
  // Profile : Define Support Points
  gp_Pnt aPnt1(-myWidth / 2., 0, 0);        
  gp_Pnt aPnt2(-myWidth / 2., -myThickness / 4., 0);
  gp_Pnt aPnt3(0, -myThickness / 2., 0);
  gp_Pnt aPnt4(myWidth / 2., -myThickness / 4., 0);
  gp_Pnt aPnt5(myWidth / 2., 0, 0);

  // Profile : Define the Geometry
  Handle(Geom_TrimmedCurve) anArcOfCircle = GC_MakeArcOfCircle(aPnt2,aPnt3,aPnt4);
  Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(aPnt1, aPnt2);
  Handle(Geom_TrimmedCurve) aSegment2 = GC_MakeSegment(aPnt4, aPnt5);

  // Profile : Define the Topology
  TopoDS_Edge anEdge1 = BRepBuilderAPI_MakeEdge(aSegment1);
  TopoDS_Edge anEdge2 = BRepBuilderAPI_MakeEdge(anArcOfCircle);
  TopoDS_Edge anEdge3 = BRepBuilderAPI_MakeEdge(aSegment2);
  TopoDS_Wire aWire  = BRepBuilderAPI_MakeWire(anEdge1, anEdge2, anEdge3);


  // Profile : Define Support Points
  gp_Pnt aPnt1b(-myWidth / 2., 0, 4);        
  gp_Pnt aPnt2b(-myWidth / 2., -myThickness / 4., 4);
  gp_Pnt aPnt3b(0, -myThickness / 2., 4);
  gp_Pnt aPnt4b(myWidth / 2., -myThickness / 4., 4);
  gp_Pnt aPnt5b(myWidth / 2., 0, 4);

  // Profile : Define the Geometry
  Handle(Geom_TrimmedCurve) anArcOfCircleb = GC_MakeArcOfCircle(aPnt2b,aPnt3b,aPnt4b);
  Handle(Geom_TrimmedCurve) aSegment1b = GC_MakeSegment(aPnt1b, aPnt2b);
  Handle(Geom_TrimmedCurve) aSegment2b = GC_MakeSegment(aPnt4b, aPnt5b);

  // Profile : Define the Topology
  TopoDS_Edge anEdge1b = BRepBuilderAPI_MakeEdge(aSegment1b);
  TopoDS_Edge anEdge2b = BRepBuilderAPI_MakeEdge(anArcOfCircleb);
  TopoDS_Edge anEdge3b = BRepBuilderAPI_MakeEdge(aSegment2b);
  TopoDS_Wire aWireb  = BRepBuilderAPI_MakeWire(anEdge1b, anEdge2b, anEdge3b);

  BRepOffsetAPI_ThruSections aTool(Standard_True);
  aTool.AddWire(aWire);
  aTool.AddWire(aWireb);
  aTool.CheckCompatibility(Standard_False);

  TopoDS_Shape myThreading = aTool.Shape();

  return myThreading;
}

int main( int argc, char ** argv ){
  TopoDS_Shape s = MakeBottle( 10.0, 10.0, 2.0 );
  return( 0 );
}

