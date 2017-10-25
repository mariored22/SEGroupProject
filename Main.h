#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace iProlog
{

	class Main
	{

  public:
	  static void println(void *const o);

	  static void pp(void *const o);

	  static void run(const std::wstring &fname0);

	  static void srun(const std::wstring &fname0);

	  static void main(std::vector<std::wstring> &args);
	};

}
