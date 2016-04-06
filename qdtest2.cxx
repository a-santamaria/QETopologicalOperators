#include "itkQuadEdgeMesh.h"
#include "itkMeshFileWriter.h"
#include <vtkGenericDataObjectReader.h>
#include <vtkStructuredGrid.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <itkQuadEdge.h>
#include <string>
#include <itkQuadEdgeMeshBaseIterator.h>
#include <itkVector.h>


#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>
#include <vtkCellArray.h>
#include <vtkLine.h>
#include <vtkStructuredGrid.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <string>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCellArray.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkTriangle.h>



using namespace std;

int main( int argc, char* argv[] )
{
    const unsigned int Dimension = 3;

    typedef double                                    CoordType;
    typedef itk::QuadEdgeMesh< CoordType, Dimension > MeshType;
    typedef itk::QuadEdgeMeshPolygonCell< MeshType::CellType > PolygonType;

    MeshType::Pointer mesh = MeshType::New();
    MeshType::Pointer copy = MeshType::New();

    typedef MeshType::PointsContainer         PointsContainer;
    typedef MeshType::PointsContainerPointer  PointsContainerPointer;

    typedef MeshType::PointIdentifier idP;
    PointsContainerPointer points = PointsContainer::New();
    points->Reserve( 100 );

    typedef MeshType::PointType               PointType;
    PointType p;
    p[2] = 0.;

    typedef MeshType::PointIdentifier         PointIdentifier;
    PointIdentifier k = 0;
  for( int i = 0; i < 10; i++ )
    {
    p[0] = static_cast< CoordType >( i );

    for( int j = 0; j < 10; j++ )
      {
      p[1] = static_cast< CoordType >( j );
      points->SetElement( k, p );
      k++;
      }
    }

  mesh->SetPoints( points );
  copy->SetPoints( points );

  k = 0;

  for( int i = 0; i < 9; i++ )
    {
    for( int j = 0; j < 9; j++ )
      {
      mesh->AddFaceTriangle( k, k+1,  k+11 );
      mesh->AddFaceTriangle( k, k+11, k+10 );
      copy->AddFaceTriangle( k, k+1,  k+11 );
      copy->AddFaceTriangle( k, k+11, k+10 );
      k++;
      }
    k++;
    }



  MeshType::FrontIterator it= mesh->BeginFront();
  MeshType::FrontIterator itC= mesh->BeginFront();
  MeshType::PointType p1, p2,p3;
  PointType pm12, pm23, pm31;
  idP idP1,idP2,idP3;

  itk::Vector<double, 3> v1,v2, v3, aux;
  v1[0] = -1;
  v1[1] = -1;
  v1[2] = -1;

  std::vector<std::vector<   PointType > >puntos;
  for (int i=0; i< mesh->GetNumberOfPoints();i++ )
  {
      puntos.push_back( vector<PointType>(mesh->GetNumberOfPoints()) );
      for(int j = 0; j < mesh->GetNumberOfPoints(); j++){
    //      std::cout << "i j " << i << " " << j << std::endl;
          puntos[i][j] = -1;
      }

  }

  for(; it != mesh->EndFront(); it++) {

    //cout<<"------------------------------------"<<endl;
    //cout << "origin " << it.Value()->GetOrigin() <<": ";

    p1 = mesh->GetPoint( it.Value()->GetOrigin() );
    v1 = p.GetVectorFromOrigin();
    idP1 =it.Value()->GetOrigin() ;

    p2 = mesh->GetPoint( it.Value()->GetDestination() );
    idP2 =it.Value()->GetDestination();

    v2 = p2.GetVectorFromOrigin();
  //  QEType* nuevo = it.Value()->GetLnext();
    p3 = mesh->GetPoint(it.Value()->GetLnext()->GetDestination());
    v3 = p3.GetVectorFromOrigin();
    idP3 = it.Value()->GetLnext()->GetDestination();

    //Encontrar el punto medio y añadirlo al mesh
    if (puntos[idP1][idP2]==-1)
    {
      pm12[0]=(v1[0]+v2[0])/2;
      pm12[1]=(v1[1]+v2[1])/2;
      pm12[2]=0;
      puntos[idP1][idP2]= pm12;
      copy->AddPoint(pm12);
    }
    else
      pm12 = puntos[idP1][idP2];
  //  MeshType::PointIdentifier idPm = copy->AddPoint(pm);

    if (puntos[idP2][idP3]==-1)
    {
      pm23[0]=(v2[0]+v3[0])/2;
      pm23[1]=(v2[1]+v3[1])/2;
      pm23[2]=0;
      puntos[idP2][idP3] = pm23;
  copy->AddPoint(pm23);
    }
    else
       pm23 = puntos[idP2][idP3];
    if(puntos[idP3][idP1]==-1)
    {
      pm31[0]=(v3[0]+v1[0])/2;
      pm31[1]=(v3[1]+v1[1])/2;
      pm31[2]=0;
      puntos[idP3][idP1] = pm31;
      copy->AddPoint(pm31);
    }
    else
      pm31 = puntos[idP3][idP1];

   copy->AddFaceTriangle( idP1, idP2,  idP3 );
  }

  //-----------------------------------------------------------------------------

  return EXIT_SUCCESS;
}
