#include "itkQuadEdgeMesh.h"
#include "itkMeshFileWriter.h"
#include <itkQuadEdge.h>
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

#include <string>

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

    for( int i = 0; i < 10; i++ ) {
        p[0] = static_cast< CoordType >( i );

        for( int j = 0; j < 10; j++ ) {
            p[1] = static_cast< CoordType >( j );
            points->SetElement( k, p );
            k++;
        }
    }

    mesh->SetPoints( points );

    k = 0;
    int cont = 0;
    for( int i = 0; i < 9; i++ )
    {
        for( int j = 0; j < 9; j++ )
        {
            mesh->AddFaceTriangle( k, k+1,  k+11 );
            cont++;
            mesh->AddFaceTriangle( k, k+11, k+10 );
            cont++;
            k++;
        }
        k++;
    }
    std::cout << "cont triangles " << cont << std::endl;

    vtkSmartPointer<vtkPoints> vpoints = vtkSmartPointer<vtkPoints>::New();
    MeshType::PointsContainer::Iterator itPoints = mesh->GetPoints()->Begin();

    for( ; itPoints != mesh->GetPoints()->End(); ++itPoints)
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
    renderer->SetBackground(.2, .3, .4);


    renderWindow->Render();
    renderWindowInteractor->Start();

    MeshType::FrontDualIterator itFace = mesh->BeginDualFront();

    for( ; itFace != mesh->EndDualFront(); itFace++) {
        std::cout << "face " << itFace.Value()->GetIdent() << std::endl;


        vtkSmartPointer<vtkTriangle> triangle =
          vtkSmartPointer<vtkTriangle>::New();
        MeshType::QEType* qe = itFace.Value()->GetRot();
        MeshType::QEType* temp = qe;

        int cont = 0;
        do {

            std::cout <<temp->GetOrigin() << " ";
            triangle->GetPointIds()->SetId( cont++, temp->GetOrigin() );
            vtkLine *line = vtkLine::New();

            line->GetPointIds()->SetId(0,(vtkIdType)(temp->GetOrigin()) );
            line->GetPointIds()->SetId(1,(vtkIdType)(temp->GetDestination()));
            edgeCells->InsertNextCell(line);

            temp = temp->GetRnext();


        } while(temp != qe);
        std::cout << std::endl;
        triangles->InsertNextCell(triangle);

        renderWindow->Finalize ();
        renderWindow->Render();
        renderWindow->Start ();

    }


  return EXIT_SUCCESS;
}
