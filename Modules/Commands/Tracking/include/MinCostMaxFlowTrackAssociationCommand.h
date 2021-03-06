/*
 * MinCostMaxFlowTrackAssociationCommand.h
 *
 *  Created on: May 20, 2014
 *      Author: morgan
 */

#ifndef MINCOSTMAXFLOWTRACKASSOCIATIONCOMMAND_H_
#define MINCOSTMAXFLOWTRACKASSOCIATIONCOMMAND_H_

#include "AppCommand.h"
#include "tttDescriptionDataTypes.h"

namespace ttt {

template<int dim> class MinCostMaxFlowTrackAssociationCommand : public AppCommand {

private:
	typename ttt::TissueDescriptor<dim>::Pointer m_ObservedTissueDescriptor;

	typename ttt::TrackedTissueDescriptor<dim>::Pointer m_PreviousTrackedTissueDescriptor;
	typename ttt::TrackedTissueDescriptor<dim>::Pointer m_CurrentTrackedTissueDescriptor;

	double m_TerminationWeight=1;
	double m_CreationWeight=0.5;
	double m_AssociationWeight=0.1;
	double m_ApoptosisWeight=1;
	double m_MitosisWeight=1;

	double m_DistanceWeight=1;
	double m_AreaWeight=0.01;
	double m_PerimeterWeight=0;
	double m_XXWeight=0;
	double m_XYWeight=0;
	double m_YYWeight=0;


	int m_K;
	int m_KMitosis;
	unsigned int m_NextID;

public:

	MinCostMaxFlowTrackAssociationCommand();
	virtual ~MinCostMaxFlowTrackAssociationCommand();

	inline void SetNextID(unsigned int nextID){
		m_NextID=nextID;
	}
	inline void SetPreviousTrackedTissueDescriptor(const typename ttt::TrackedTissueDescriptor<dim>::Pointer & previousTrackedTissueDescriptor){
		m_PreviousTrackedTissueDescriptor=previousTrackedTissueDescriptor;
	}

	inline void SetObservedTissueDescriptor(const typename ttt::TissueDescriptor<dim>::Pointer & observedTissueDescriptor ){
		m_ObservedTissueDescriptor=observedTissueDescriptor;
	}

	inline typename ttt::TrackedTissueDescriptor<dim>::Pointer GetCurrentTrackedTissueDescriptor(){
		return m_CurrentTrackedTissueDescriptor;
	}
	inline void SetTerminationWeight(double weight){
		m_TerminationWeight=weight;
	}
	inline void SetCreationWeight(double weight){
		m_CreationWeight=weight;
	}
	inline void SetAssociationWeight(double weight){
		m_AssociationWeight=weight;
	}
	inline void SetMitosisWeight(double weight){
		m_MitosisWeight=weight;
	}
	inline void SetApoptosisWeight(double weight){
		m_ApoptosisWeight=weight;
	}


	inline void SetDistanceWeight(double weight){
		m_DistanceWeight=weight;
	}
	inline void SetAreaWeight(double weight){
		m_AreaWeight=weight;
	}
	inline void SetPerimeterWeight(double weight){
		m_PerimeterWeight=weight;
	}
	inline void SetXXWeight(double weight){
		m_XXWeight=weight;
	}
	inline void SetXYWeight(double weight){
		m_XYWeight=weight;
	}
	inline void SetYYWeight(double weight){
		m_YYWeight=weight;
	}
	inline void SetK(unsigned int K){
		m_K=K;
	}
	inline void SetKMitosis(unsigned int K){
		m_KMitosis=K;
	}
	virtual void Do();

};

} /* namespace ttt */
#include "MinCostMaxFlowTrackAssociationCommand.hxx"
#endif /* MINCOSTMAXFLOWTRACKASSOCIATIONCOMMAND_H_ */
