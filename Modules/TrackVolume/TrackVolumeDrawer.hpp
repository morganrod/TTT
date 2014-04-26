/*
 * TrackVolumeDrawer.hpp
 *
 *  Created on: Apr 16, 2014
 *      Author: morgan
 */

#ifndef TRACKVOLUMEDRAWER_HPP_
#define TRACKVOLUMEDRAWER_HPP_
#include <vtkActor.h>
#include <vtkOpenGLPolyDataMapper.h>
#include <vtkLineSource.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>
#include <vtkTIFFWriter.h>
#include <vtkPNGWriter.h>
void DrawTrackVolume(const std::string & fileName,std::vector<ttt::TrackedTissueDescriptor::Pointer> & tissues){
	vtkSmartPointer<vtkRenderer> renderer= vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow= vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor= vtkSmartPointer<vtkRenderWindowInteractor>::New();

	renderWindow->AddRenderer(renderer);
	renderWindowInteractor->SetRenderWindow(renderWindow);


	ttt::TrackedTissueDescriptor::Pointer currentTissue=tissues[0];

	typedef std::vector<double> Color ;

	std::map<int,Color> m_TrackID2Color;


	for(int t=0;t<tissues.size()-1;t++){
	//for(int t=0;t<m_Project.GetNumFrames()-1;t++){
	//for(int t=0;t<2;t++){

		ttt::TrackedTissueDescriptor::Pointer nextTissue =tissues[t+1];

		BGL_FORALL_VERTICES(v,*(currentTissue->m_CellGraph),ttt::TrackedCellGraph){


			int ID=boost::get(ttt::TrackedCellPropertyTag(),(*currentTissue->m_CellGraph),v).GetID();

			std::vector<double> color;
			if(m_TrackID2Color.count(ID)){
				color=m_TrackID2Color[ID];
			}else{

			  //color=generator.generateRandomColor(rand());
			  color.push_back((double)rand()/RAND_MAX );
			  color.push_back((double)rand()/RAND_MAX );
			  color.push_back((double)rand()/RAND_MAX );
			  m_TrackID2Color[ID]=color;
		  }

			ttt::TrackedCellVertexType nextV=ttt::CellID2VertexDescriptor(ID,nextTissue);
			if(nextV!=-1){

				itk::Point<double,3> a =boost::get(ttt::TrackedCellPropertyTag(),(*currentTissue->m_CellGraph),v).GetCentroid();
				itk::Point<double,3> b =boost::get(ttt::TrackedCellPropertyTag(),(*nextTissue->m_CellGraph),nextV).GetCentroid();

				vtkSmartPointer<vtkLineSource> line=vtkSmartPointer<vtkLineSource>::New();

				line->SetPoint1(a[0],a[1],3*t);
				line->SetPoint2(b[0],b[1],3*(t+1));

				line->Update();
				vtkSmartPointer<vtkOpenGLPolyDataMapper> mapper = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
				mapper->SetInputConnection(line->GetOutputPort());
				vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

				actor->GetProperty()->SetLineWidth(5);
				actor->GetProperty()->SetColor(color[0],color[1],color[2]);
				actor->SetMapper(mapper);


				renderer->AddActor(actor);
			}else{
				std::pair<ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType,ttt::TrackedTissueDescriptor::DualGraphVertexDescriptorType> children=CellParentID2VertexDescriptor(ID,nextTissue);
				if(children.first==-1 || children.second==-1) continue;

				itk::Point<double,3> a =boost::get(ttt::TrackedCellPropertyTag(),(*currentTissue->m_CellGraph),v).GetCentroid();
				itk::Point<double,3> b =boost::get(ttt::TrackedCellPropertyTag(),(*nextTissue->m_CellGraph),children.first).GetCentroid();

				itk::Point<double,3> c =boost::get(ttt::TrackedCellPropertyTag(),(*currentTissue->m_CellGraph),v).GetCentroid();
				itk::Point<double,3> d =boost::get(ttt::TrackedCellPropertyTag(),(*nextTissue->m_CellGraph),children.second).GetCentroid();

				int IDA=boost::get(ttt::TrackedCellPropertyTag(),(*nextTissue->m_CellGraph),children.first).GetID();
				int IDB=boost::get(ttt::TrackedCellPropertyTag(),(*nextTissue->m_CellGraph),children.second).GetID();

				m_TrackID2Color[IDA]=color;
				m_TrackID2Color[IDB]=color;


				vtkSmartPointer<vtkLineSource> lineA=vtkSmartPointer<vtkLineSource>::New();

				lineA->SetPoint1(a[0],a[1],3*t);
				lineA->SetPoint2(b[0],b[1],3*(t+1));
				lineA->Update();
				vtkSmartPointer<vtkOpenGLPolyDataMapper> mapperA = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
				mapperA->SetInputConnection(lineA->GetOutputPort());
				vtkSmartPointer<vtkActor> actorA = vtkSmartPointer<vtkActor>::New();

				actorA->GetProperty()->SetColor(color[0],color[1],color[2]);
				actorA->SetMapper(mapperA);
				actorA->GetProperty()->SetLineWidth(5);

				renderer->AddActor(actorA);

				vtkSmartPointer<vtkLineSource> lineB=vtkSmartPointer<vtkLineSource>::New();

				lineB->SetPoint1(c[0],c[1],3*t);
				lineB->SetPoint2(d[0],d[1],3*(t+1));
				lineB->Update();
				vtkSmartPointer<vtkOpenGLPolyDataMapper> mapperB = vtkSmartPointer<vtkOpenGLPolyDataMapper>::New();
				mapperB->SetInputConnection(lineB->GetOutputPort());
				vtkSmartPointer<vtkActor> actorB = vtkSmartPointer<vtkActor>::New();

				actorB->GetProperty()->SetColor(color[0],color[1],color[2]);
				actorB->SetMapper(mapperB);

				actorB->GetProperty()->SetLineWidth(5);

				renderer->AddActor(actorB);



			}
		}
		currentTissue=nextTissue;

	}
	renderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel
	renderWindow->Render();

	vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =vtkSmartPointer<vtkWindowToImageFilter>::New();
	windowToImageFilter->SetInput(renderWindow);
	windowToImageFilter->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
	windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
	windowToImageFilter->Update();

	vtkSmartPointer<vtkTIFFWriter> writer = vtkSmartPointer<vtkTIFFWriter>::New();
	writer->SetFileName(fileName.c_str());
	writer->SetInputConnection(windowToImageFilter->GetOutputPort());
	writer->Write();

	renderWindowInteractor->Start();

}

#endif /* TRACKVOLUMEDRAWER_HPP_ */