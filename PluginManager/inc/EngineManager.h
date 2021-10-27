/*
 * EngineManager.h
 *
 *  Created on: Jan 15, 2017
 *      Author: meditation
 */

#ifndef ENGINEMANAGER_H_
#define ENGINEMANAGER_H_

namespace RLMEngineManager
{
	class EngineManager
	{
	public:
		static EngineManager			&getInstance();
		void							scan();

		void							InitPlugin();

	private:
										EngineManager();
		virtual 						~EngineManager();


	};

}

#endif
