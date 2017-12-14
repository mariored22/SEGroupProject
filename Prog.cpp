#include <string>
#include <sstream>
#include "Prog.h"
#include "Main.h"


Prog::Prog(const char* fname) : Engine(fname)
{
}

Prog::~Prog()
{
}

void Prog::ppCode()
{
	pp("\nSYMS:");
	std::stringstream ss1;

	ss1 << "{";
	for(std::vector<std::string>::iterator sl = slist.begin(); sl != slist.end(); sl++)
	{
		if (sl != slist.begin())
		{
			ss1 << ", ";
		}
		ss1 << *sl << "=" << syms[*sl];
		
	}
	ss1 << "}\n";
    	pp(ss1.str());
	pp("\nCLAUSES:\n");

	for (int i = 0; i < (int)clauses.size(); i++) 
	{
		std::stringstream ss2;
		ss2 << "[" << i << "]:";
		ss2 << showClause(clauses[i]);      
		pp(ss2.str());
	}
	pp("");
}

std::string Prog::showClause(const Clause& s) 
{
	std::stringstream buf;
	int l = s.hgsLength;
	buf << "---base:[" << s.base << "] neck: " << s.neck << "-----" << std::endl;
	buf << showCells(s.base, s.len); // TODO
	buf << std::endl;
	buf << showCell(s.hgs[0]);

	buf << " :- [";
	for (int i = 1; i < l; i++) 
	{
		int e = s.hgs[i];
		buf << showCell(e);
		if (i < l - 1) 
		{
			buf << ", ";
		}
	}

    	buf << "]" << std::endl;

	buf << showTerm(s.hgs[0]);
	if (l > 1) 
	{
		buf << " :- " << std::endl;
		for (int i = 1; i < l; i++) 
		{
			int e = s.hgs[i];
			buf << "  ";
			buf << showTerm(e);
			buf << std::endl;
		}
	}
	else
	{
		buf << std::endl;
	}
	return buf.str();
}

std::string Prog::st0(int x)
{
	std::stringstream res;

	return res.str();
}

bool isOp(const std::string& name)
{
	return (name=="/") || (name=="-") || (name=="+") || (name=="=");
}

bool isListCons(const std::string& name)
{
	return (name==".") || (name=="[|]") || (name=="list");
}

std::string Prog::exportTerm(int x, bool& isArray)
{
	std::stringstream res;

	x = deref(x);

	int t = tagOf(x);
	int w = detag(x);
    
	switch (t) 
	{
		case C:
			isArray = false;
			res << getSym(w);
		break;
		case N:
			isArray = false;
			res << w;
		break;
		case V:
			//case U:
			isArray = false;
			res << "V" << w;
		break;
		case R: 
			{			
				int a = heap[w];
				if (A != tagOf(a))
				{
					return "*** should be A, found=" + showCell(a);
				}
				int n = detag(a);
				//final Object[] arr = new Object[n];
				int k = w + 1;

				std::string name = exportTerm(heap[k], isArray);
								
				if (n==3 && isOp(name))
				{
					std::string second = exportTerm(heap[k+1], isArray);
					std::string third = exportTerm(heap[k+2], isArray);

					res << "(" << second << name << third << ")";
				}
				else if (n==3 && isListCons(name)) 
				{
					std::string second = exportTerm(heap[k+1], isArray);
					std::string third = exportTerm(heap[k+2], isArray);
																		
					if (third!="nil")
					{
						if (third[0]=='[')
						{														
							res << "[" << second << "," << third.substr(1,third.size()-2) << "]";
						}
						else
							res << "[" << second << "|" << third << "]";
					}
					else 
					{
						res << "[" << second << "]";
					}	

					isArray = true;
				}
				else if (n==2 && name=="$VAR") 
				{
					res << "_"  << exportTerm(heap[k+1], isArray);
				}
				else
				{
					res << name << "(";
					for (int i = 1; i < n; i++) 
					{
						if (i>1)
							res << ",";
						res << exportTerm(heap[k + i], isArray);
					}
					isArray = true;
					res << ")";
				}
			      
		break;
		default:
			res << "*BAD TERM*" << showCell(x);
		break;
    }

    return res.str();
}
