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
    typedef itk::QuadEdge QEType;
    typedef itk::GeometricalQuadEdge< unsigned long int, unsigned long int, bool, bool, true> GQEType;


    MeshType::Pointer mesh = MeshType::New();
    MeshType::Pointer copy = MeshType::New();

    typedef MeshType::PointsContainer         PointsContainer;
    typedef MeshType::PointsContainerPointer  PointsContainerPointer;

    typedef MeshType::PointIdentifier idP;
    PointsContainerPointer points = PointsContainer::New();
    PointsContainerPointer pointsC = PointsContainer::New();
    points->Reserve( 100 );
    pointsC->Reserve(100);

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
      pointsC->SetElement(k, p);
      k++;
      }
    }

  mesh->SetPoints( points );
  copy->SetPoints( pointsC );

  int voy = k;
  std::cout << "voy esta en "<< voy << std::endl;
  k = 0;

  for( int i = 0; i < 9; i++ )
    {
    for( int j = 0; j < 9; j++ )
      {
      mesh->AddFaceTriangle( k, k+1,  k+11 );
      mesh->AddFaceTriangle( k, k+11, k+10 );
    //  copy->AddFaceTriangle( k, k+1,  k+11 );
    //  copy->AddFaceTriangle( k, k+11, k+10 );
      k++;
      }
    k++;
    }

  itk::Vector<double, 3> v1,v2, v3, aux;
  v1[0] = -1;
  v1[1] = -1;
  v1[2] = -1;

  std::vector<std::vector< std::pair<idP,PointType> > >puntos;
  for (int i=0; i< mesh->GetNumberOfPoints();i++ )
  {
      puntos.push_back( vector<std::pair<idP,PointType> >(mesh->GetNumberOfPoints()) );
      for(int j = 0; j < mesh->GetNumberOfPoints(); j++){
    //      std::cout << "i j " << i << " " << j << std::endl;
        idP aux;
        std::pair<idP,PointType> p=std::make_pair (aux,-1);
          puntos[i][j] = p;
      }

  }


  MeshType::FrontDualIterator it= mesh->BeginDualFront();
  MeshType::FrontDualIterator itC= mesh->BeginDualFront();
  MeshType::PointType p1, p2,p3;
  PointType pm12, pm23, pm31;
  idP idP1,idP2,idP3, idm1, idm2, idm3;

     //voy = 0;
     int cont = 0;

    for(; it != mesh->EndDualFront() && cont++ < 4; it++) {

        GQEType*  qe = it.Value()->GetRot()->GetSym();
        GQEType* temp = qe;
/*
        do {
            p1 = mesh->GetPoint( qe->GetOrigin() );
            std::cout << qe->GetOrigin() << " " << p1.GetVectorFromOrigin() << " -> ";
            qe = qe->GetLnext();
        } while (temp != qe);
        std::cout << std::endl;
*/

        p1 = mesh->GetPoint( qe->GetOrigin() );
        v1 = p1.GetVectorFromOrigin();
        idP1 = qe->GetOrigin() ;

        p2 = mesh->GetPoint( qe->GetDestination() );
        idP2 = qe->GetDestination();
        v2 = p2.GetVectorFromOrigin();
        //  QEType* nuevo = it.Value()->GetLnext();
        p3 = mesh->GetPoint( qe->GetLnext()->GetDestination() );
        v3 = p3.GetVectorFromOrigin();
        idP3 = qe->GetLnext()->GetDestination();
        std::cout << "v1 " << v1 << " v2 " << v2 << " v3 " << v3 << std::endl;
        //std::cout << "idp1 " << idP1 << " idp1 " << idP2 << " idp3 " << idP3 << std::endl;
        //Encontrar el punto medio y añadirlo al mesh
    //    std::cout << "secobd "<<puntos[idP1][idP2].second << std::endl;
    /*    copy->AddPoint(p1);
        copy->AddPoint(p2);
        copy->AddPoint(p3);
*/
        if ( puntos[idP1][idP2].second == -1 )
        {
            std::cout << "los puntos" <<v1<<" "<<v2<<std::endl;
            pm12[0]=(v1[0]+v2[0])/2;
            pm12[1]=(v1[1]+v2[1])/2;
            pm12[2]=0;
            idm1 = copy->AddPoint(pm12);
        std::cout<<" puntos medios "<<pm12 << " con id " << idm1<<std::endl;
            puntos[idP1][idP2]=std::make_pair(idm1,pm12);
            puntos[idP2][idP1]=std::make_pair(idm1,pm12);
    //        std::cout << "secobd "<<puntos[idP1][idP2].second << std::endl;
            pointsC->SetElement(voy++, pm12);
        }
        else
        {
            std::cout << "------------------->>entre else" << std::endl;
            pm12 = puntos[idP1][idP2].second;
            idm1 = puntos[idP1][idP2].first;
    //        std::cout << "entree al els" << std::endl;
        }
        //  MeshType::PointIdentifier idPm = copy->AddPoint(pm);

        if (puntos[idP2][idP3].second==-1)
        {
            std::cout << "los puntos" <<v2<<" "<<v3<<std::endl;
            pm23[0]=(v2[0]+v3[0])/2;
            pm23[1]=(v2[1]+v3[1])/2;
            pm23[2]=0;

            pointsC->SetElement(voy++, pm23);
            idm2 = copy->AddPoint(pm23);
            puntos[idP2][idP3]=std::make_pair(idm2,pm23);
            puntos[idP2][idP3]=std::make_pair(idm2,pm23);
            std::cout<<" puntos medios "<<pm23<< " con id " << idm2 << std::endl;
        }
        else
        {
            std::cout << "------------------->>entre else" << std::endl;
            pm23 = puntos[idP2][idP3].second;
            idm2 = puntos[idP2][idP3].first;
        }
        if(puntos[idP3][idP1].second==-1)
        {
            std::cout << "los puntos" <<v3<<" "<<v1<<std::endl;
            pm31[0]=(v3[0]+v1[0])/2;
            pm31[1]=(v3[1]+v1[1])/2;
            pm31[2]=0;

            pointsC->SetElement(voy++, pm31);
            idm3 = copy->AddPoint(pm31);
        std::cout<<" puntos medios "<<pm31<< " con ide " << idm3 <<std::endl;
            puntos[idP3][idP1]=std::make_pair(idm3,pm31);
            puntos[idP1][idP3]=std::make_pair(idm3,pm31);
        }
        else
        {
            std::cout << "------------------->>entre else" << std::endl;
            pm31 = puntos[idP3][idP1].second;
            idm3 = puntos[idP3][idP1].first;
        }
       //copy->SetPoints( pointsC );
        //copy->SetPoints( pointsC );
        std::cout << "pm12 " << pm12 << " pm23 " << pm23 << " pm31 " << pm31 << std::endl;
        std::cout << "idp1 " << idm1 << " idp2 " << idm2 << " idp3 " << idm3 << std::endl;
        std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&num triangles " << copy->GetNumberOfCells() << std::endl;
        //std::cout << "vou a a gregar " << std::endl;
        copy->AddFaceTriangle( idm1, idm2,  idm3 );
        copy->AddFaceTriangle( idm1, idm3,  idP1 );
        copy->AddFaceTriangle( idm3, idm2,  idP3 );
        copy->AddFaceTriangle( idm2, idm1,  idP2 );

    }
    //copy->SetPoints( pointsC );
    //copy->SetPoints( pointsC );
  //-----------------------------------------------------------------------------

  vtkSmartPointer<vtkPoints> vpoints = vtkSmartPointer<vtkPoints>::New();
  MeshType::PointsContainer::Iterator itPoints = copy->GetPoints()->Begin();

  for( ; itPoints != copy->GetPoints()->End(); ++itPoints)
  {
      // Get the point index from the point container iterator
      int idx = itPoints->Index();
      PointType ps = itPoints->Value().GetDataPointer();
      vpoints->InsertPoint( idx, (itPoints->Value().GetDataPointer()));
  }


  vtkSmartPointer<vtkCellArray> edgeCells = vtkSmartPointer<vtkCellArray>::New();
  vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();

  vtkSmartPointer<vtkPolyData> pd =
  vtkSmartPointer<vtkPolyData>::New();
  pd->SetPoints(vpoints);
  //pd->SetLines(edgeCells);
  //pd->SetPolys(triangles);

  std::cout << "polys " << pd->GetNumberOfCells() << std::endl;

  vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter =
      vtkSmartPointer<vtkVertexGlyphFilter>::New();
  vertexFilter->SetInputData(pd);
  vertexFilter->Update();

  vtkSmartPointer<vtkPolyData> polyData =
  vtkSmartPointer<vtkPolyData>::New();
  polyData->ShallowCopy(vertexFilter->GetOutput());

  polyData->SetPolys(triangles);

  vtkSmartPointer<vtkPolyDataMapper> mapper =
  vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputData(polyData);


  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetPointSize(10);


  vtkSmartPointer<vtkRenderer> renderer =
  vtkSmartPointer<vtkRenderer>::New();

  vtkSmartPointer<vtkRenderWindow> renderWindow =
  vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize( 600, 600 );

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
  vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);
  renderer->SetBackground(.2, .3, .4);


  renderWindow->Render();
  renderWindowInteractor->Start();

  MeshType::FrontDualIterator itFace = copy->BeginDualFront();
 int cont2 = 0;
 std::cout << std::endl;
  for( ; itFace != copy->EndDualFront(); itFace++) {
      //std::cout << "face " << itFace.Value()->GetIdent() << std::endl;

      std::cout << "voy en " << cont2++ << std::endl;
      vtkSmartPointer<vtkTriangle> triangle =
        vtkSmartPointer<vtkTriangle>::New();
      MeshType::QEType* qe = itFace.Value()->GetRot();
      MeshType::QEType* temp = qe;

      int cont = 0;
      do {

          //std::cout <<temp->GetOrigin() << " ";
          triangle->GetPointIds()->SetId( cont++, temp->GetOrigin() );
          vtkLine *line = vtkLine::New();

          line->GetPointIds()->SetId(0,(vtkIdType)(temp->GetOrigin()) );
          line->GetPointIds()->SetId(1,(vtkIdType)(temp->GetDestination()));
          edgeCells->InsertNextCell(line);

          temp = temp->GetRnext();


      } while(temp != qe);
      //std::cout << std::endl;
      triangles->InsertNextCell(triangle);

      renderWindow->Finalize ();
      renderWindow->Render();
      renderWindow->Start ();

  }
  renderWindow->Finalize ();
  renderWindow->Render();
  renderWindow->Start ();
  int nn;
  cin>>nn;


  return EXIT_SUCCESS;
}
