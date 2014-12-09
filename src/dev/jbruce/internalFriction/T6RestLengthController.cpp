/*
 * Copyright © 2012, United States Government, as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All rights reserved.
 * 
 * The NASA Tensegrity Robotics Toolkit (NTRT) v1 platform is licensed
 * under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0.
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 */

/**
 * @file T6RestLengthController.cpp
 * @brief Implementation of a rest length controller for T6Model.
 * @author Drew Sabelhaus and Brian Tietz
 * @version 1.0.0
 * $Id$
 */

// This module
#include "T6RestLengthController.h"
// This application
#include "T6Model.h"
// This library
#include "core/tgLinearString.h"
#include "core/tgBulletSpringCable.h"
#include "tgcreator/tgUtil.h"
// The C++ Standard Library
#include <cassert>
#include <stdexcept>
#include <vector>
#include <iostream>

T6RestLengthController::T6RestLengthController(T6Model* subject, const double restLengthDiff) :
    m_restLengthDiff(restLengthDiff),
	m_controllerMuscleRatio(subject->muscleRatio())
{
    if (restLengthDiff < 0.0)
    {
        throw std::invalid_argument("You tried to push a rope!");
    }
}

void T6RestLengthController::onSetup(T6Model& subject)
{
    // Do a one-time update of all cable rest lengths
    // First, get all muscles (cables)

	const std::vector<tgLinearString*> passiveMuscles = subject.getPassiveMuscles();
	const std::vector<tgLinearString*> activeMuscles = subject.getActiveMuscles();

	// then, iterate over all muscles
	for (size_t i = 0; i < passiveMuscles.size(); ++i)
	{
		tgLinearString * const pMuscle = passiveMuscles[i];
		assert(pMuscle != NULL);

		// set rest length of the i-th muscle
		double desiredRestLength = pMuscle->getStartLength() - 0.7;
		pMuscle->setRestLengthSingleStep(desiredRestLength);
	}
	for (size_t i = 0; i < activeMuscles.size(); ++i)
	{
		tgLinearString * const pMuscle = activeMuscles[i];
		assert(pMuscle != NULL);

		// set rest length of the i-th muscle
		double desiredRestLength = pMuscle->getStartLength() - 0.22;
		pMuscle->setRestLengthSingleStep(desiredRestLength);
	}
}

void T6RestLengthController::onStep(T6Model& subject, double dt)
{
	static int count = 0;
	const std::vector<tgLinearString*> strings = subject.getAllMuscles();
	/// @todo consider whether this is really necessary, or of tgLinearString has all of the functions you need _BTM
	const tgBulletSpringCable* muscles;

	if(count > 100)
	{
		for(size_t i=0; i<strings.size(); i++)
		{
			std::cout << (strings[i]->getTension())/10 << "\t";
		}
		std::cout << "\n";
		for(size_t i=0; i<strings.size(); i++)
			{
				std::cout << (strings[i]->getCurrentLength())/10 << "\t";
			}
		std::cout << "\n";
//		for(size_t i=0; i<strings.size(); i++)
//			muscles = strings[i]->getMuscle();
//			{
//				std::cout << getTransform(muscles->anchor1->getWorldPosition() - muscles->anchor2->getWorldPosition()) << "\t";
//			}
//		std::cout << "\n";
	count = 0;
	}
	else
	{
		count++;
	}

      // Nothing!!
}
