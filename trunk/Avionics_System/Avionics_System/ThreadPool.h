#pragma once

#if !defined _THREADPOOL_H_
#define _THREADPOOL_H_

namespace Punica
{
	class CTPWorkItem
	{
		
	};

	class CThreadPool
	{
	public:
		CThreadPool();
		~CThreadPool();
	public:
		BOOL Create();
	};
}