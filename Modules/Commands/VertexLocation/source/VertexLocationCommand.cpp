/*
 * VertexLocationCommand.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: morgan
 */

#include "VertexLocationCommand.h"

void ttt::VertexLocationCommand::Do(){
	itk::Size<3> radius;
	radius.Fill(6);
	m_VertexLocator->SetRadius(radius);
	m_VertexLocator->SetThreshold(m_LocalMaxThreshold);
	m_VertexLocator->SetInput(m_Input);
	m_VertexLocator->GenerateData();//FIXME



	m_VertexLocations=m_VertexLocator->GetOutput();


}
