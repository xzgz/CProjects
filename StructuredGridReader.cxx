
#include <vtkSmartPointer.h>
#include <vtkGenericDataObjectReader.h>
#include "vtkStructuredPointsReader.h"
#include <vtkPolyDataMapper.h>
#include <vtkStructuredGridReader.h>
#include <vtkStructuredGridGeometryFilter.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkOutlineFilter.h>
#include <vtkCamera.h>

int main(int argc, char* argv[])
{
  // Verify input arguments
 /* if ( argc != 2 )
    {
    std::cout << "Usage: " << argv[0]
              << " Filename(.vtk)" << std::endl;
    return EXIT_FAILURE;
    }*/
  //brain_sujet01_surface0
  std::string inputFilename = "../../../../Data/brain_sujet01_surface0.vtk";//voronoi_sujet01_surface0.vtk";//argv[1];

	//vtkSmartPointer<vtkRenderer> aRenderer =
	//	vtkSmartPointer<vtkRenderer>::New();
	//vtkSmartPointer<vtkRenderWindow> renWin =
	//	vtkSmartPointer<vtkRenderWindow>::New();
	//renWin->AddRenderer(aRenderer);

	//vtkSmartPointer<vtkRenderWindowInteractor> iren =
	//	vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//iren->SetRenderWindow(renWin);
  
  // Read the file
  vtkSmartPointer<vtkGenericDataObjectReader> reader =
	vtkSmartPointer<vtkGenericDataObjectReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();

  // Visualize
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());
  mapper->ScalarVisibilityOff();

  vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetDiffuseColor(1, 1, 1);
  //actor->GetProperty()->SetColor(0.6, 0.6, 0.6);
  /*actor->GetProperty()->SetSpecular(.3);
  actor->GetProperty()->SetSpecularPower(20);
  actor->GetProperty()->SetOpacity(.5);*/

  //vtkSmartPointer<vtkOutlineFilter> outlineData =
	 // vtkSmartPointer<vtkOutlineFilter>::New();
  //outlineData->SetInputConnection(reader->GetOutputPort());

  //vtkSmartPointer<vtkPolyDataMapper> mapOutline =
	 // vtkSmartPointer<vtkPolyDataMapper>::New();
  //mapOutline->SetInputConnection(outlineData->GetOutputPort());

  //vtkSmartPointer<vtkActor> outline =
	 // vtkSmartPointer<vtkActor>::New();
  //outline->SetMapper(mapOutline);
  //outline->GetProperty()->SetColor(0, 0, 0);

  //vtkSmartPointer<vtkCamera> aCamera =
	 // vtkSmartPointer<vtkCamera>::New();
  //aCamera->SetViewUp(0, 0, -1);
  //aCamera->SetPosition(0, -1, 0);
  //aCamera->SetFocalPoint(0, 0, 0);
  //aCamera->ComputeViewPlaneNormal();
  //aCamera->Azimuth(30.0);
  //aCamera->Elevation(30.0);

  //aRenderer->AddActor(outline);
  //aRenderer->AddActor(actor);
  //aRenderer->SetActiveCamera(aCamera);
  //aRenderer->ResetCamera();
  //aCamera->Dolly(1.5);

  //// Set a background color for the renderer and set the size of the
  //// render window (expressed in pixels).
  //aRenderer->SetBackground(.2, .3, .4);
  //renWin->SetSize(640, 480);

  //// Note that when camera movement occurs (as it does in the Dolly()
  //// method), the clipping planes often need adjusting. Clipping planes
  //// consist of two planes: near and far along the view direction. The
  //// near plane clips out objects in front of the plane; the far plane
  //// clips out objects behind the plane. This way only what is drawn
  //// between the planes is actually rendered.
  //aRenderer->ResetCameraClippingRange();

  //// Initialize the event loop and then start it.
  //iren->Initialize();
  //iren->Start();

  //return EXIT_SUCCESS;

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);
  renderer->SetBackground(.2, .3, .4);

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
