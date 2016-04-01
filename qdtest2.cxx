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

    typedef MeshType::PointsContainer         PointsContainer;
    typedef MeshType::PointsContainerPointer  PointsContainerPointer;
    typedef itk::QuadEdge QEType;
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

  k = 0;

  for( int i = 0; i < 9; i++ )
    {
    for( int j = 0; j < 9; j++ )
      {
      mesh->AddFaceTriangle( k, k+1,  k+11 );
      mesh->AddFaceTriangle( k, k+11, k+10 );
      k++;
      }
    k++;
    }

/*  -------------------------------------------------------------------------------------------------

     vtkSmartPointer<vtkPoints> vpoints = vtkSmartPointer<vtkPoints>::New();
    MeshType::PointsContainer::Iterator itPoints = mesh->GetPoints()->Begin();

    for( ; itPoints != mesh->GetPoints()->End(); ++itPoints)
    {
        // Get the point index from the point container iterator
        int idx = itPoints->Index();
        PointType ps = itPoints->Value().GetDataPointer();
        vpoints->InsertPoint( idx, (itPoints->Value().GetDataPointer()));
    }

    MeshType::FrontIterator it = mesh->BeginFront();
    MeshType::CellsContainerPointer cells = mesh->GetCells();

    vtkSmartPointer<vtkCellArray> edgeCells = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkCellArray> triangles = vtkSmartPointer<vtkCellArray>::New();
    for(; it != mesh->EndFront(); it++) {
        //cout<<"------------------------------------"<<endl;
        std::cout << " edge " << it.Value()->GetIdent();
        std::cout << " origin " << it.Value()->GetOrigin();
        std::cout << " dest" << it.Value()->GetDestination();
        MeshType::CellIdentifier idFace = it.Value()->GetLeft();
        PolygonType* face = dynamic_cast< PolygonType* >( cells->ElementAt( idFace ) );

        if( face ) {
            MeshType::QEType *qe = face->GetEdgeRingEntry();
            MeshType::QEType *temp = qe;

            unsigned int k = 0;
            vtkSmartPointer<vtkTriangle> triangle =
                vtkSmartPointer<vtkTriangle>::New();
            int cont = 0;
            do {

                //std::cout <<temp->GetOrigin() << " ";
                triangle->GetPointIds()->SetId( cont++, temp->GetOrigin() );
                vtkLine *line = vtkLine::New();

                line->GetPointIds()->SetId(0,(vtkIdType)(temp->GetOrigin()) );
                line->GetPointIds()->SetId(1,(vtkIdType)(temp->GetDestination()));
                edgeCells->InsertNextCell(line);

                temp = temp->GetLnext();


            } while(temp != qe);
            //std::cout << std::endl;
            triangles->InsertNextCell(triangle);


        }
        cout<< " id face " << idFace << endl << endl;
        //cout<<"------------------------------------"<<endl;
    }


    vtkSmartPointer<vtkPolyData> pd =
    vtkSmartPointer<vtkPolyData>::New();
    pd->SetPoints(vpoints);
    //pd->SetLines(edgeCells);
    pd->SetPolys(triangles);

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
    mapper->SetInputData(pd);


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
    // renderer->SetBackground(.2, .3, .4);


    renderWindow->Render();
    renderWindowInteractor->Start();



/*/


  MeshType::FrontIterator it= mesh->BeginFront();
itk::Vector<double, 3> v1,v2;

  for(; it != mesh->EndFront(); it++) {
    
    //cout<<"------------------------------------"<<endl;
    //cout << "origin " << it.Value()->GetOrigin() <<": ";
    MeshType::PointType p = mesh->GetPoint( it.Value()->GetOrigin() );
    v1 = p.GetVectorFromOrigin();
    //cout << v1[0] << " " << v1[1] << " " << v1[2] << endl;

    MeshType::CellIdentifier id = it.Value()->GetLeft();


    //cout << " dest " << it.Value()->GetDestination() << endl;
    p = mesh->GetPoint( it.Value()->GetDestination() );
    v2 = p.GetVectorFromOrigin();
    //cout << v2[0] << " " << v2[1] << " " << v2[2] << endl;

    //Encontrar el punto medio y añadirlo al mesh
    PointType pm;
    pm[0]=(v1[0]+v2[0])/2;
    pm[1]=(v1[1]+v2[1])/2;
    pm[2]=0;
    MeshType::PointIdentifier idPm = mesh->AddPoint(pm);

    it.Value()->Splice(it.Value()->GetLnext());
    
    QEType* nuevo = mesh->AddEdge( idPm, it.Value()->GetDestination() ); 

    it.Value()->GetSym()->Splice( nuevo );   

  cout<< " id face " << id << endl;
    //cout<<"------------------------------------"<<endl;

  }

  //-----------------------------------------------------------------------------  

  return EXIT_SUCCESS;
}


