#pragma once

#include <string>
#include <vector>

namespace iProlog
{
	class IntList
	{

  private:
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
	  const int head_Renamed;
//JAVA TO C++ CONVERTER NOTE: Fields cannot have the same name as methods:
	  IntList *const tail_Renamed;

  public:
	  virtual ~IntList()
	  {
		  delete tail;
	  }

  private:
	  IntList(int const head);

	  IntList(int const X, IntList *const Xs);

  public:
	  static bool isEmpty(IntList *const Xs);

	  static int head(IntList *const Xs);

	  static constexpr IntList *empty = nullptr;

	  static IntList *tail(IntList *const Xs);

	  static IntList *cons(int const X, IntList *const Xs);

	  static IntList *app(std::vector<int> &xs, IntList *const Ys);

	  static IntStack *toInts(IntList *Xs);

	  static int len(IntList *const Xs);

	  std::wstring toString() override;
	};

}
