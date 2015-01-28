#include "JHerwigDataManager.h"
#include <set>
#include <map>
#include <vector>
#include <list>

using namespace std;

//is a particle with ID a parent of current particle B?
bool JHerwigDataManager::isParentID(int IDA, ThePEG::tPPtr B) { 
  bool istrue = 0;
  Lorentz5Momentum Parent5Mom, Amom, Bmom; 

  Bmom = (B->final())->momentum();

  if((Bmom.e())/GeV == 0) { cout << "error: particle B has zero energy!" << endl; exit(1); }


  ThePEG::tPPtr test_parent, test_current, test_current2;
  test_current = B;
  test_current2 = B;
  Bmom = test_current->momentum();
  
  

    while(test_current->parents().size() != 0) { 

    if( test_current->id() == IDA ) { istrue = 1;}
    if( test_current2->id() == IDA ) { istrue = 1;}
   

    if(test_current->parents().size() != 0) { 
      test_current = test_current->parents()[0]; 
    }
    if(test_current->parents().size() == 2) { 
      test_current2 = test_current->parents()[1]; 
    } 
    
  }

  test_current = B;
  test_current2 = B;
  
  while(test_current->parents().size() != 0) { 
    
    if( test_current->id() == IDA ) { istrue = 1;}
    if( test_current2->id() == IDA ) { istrue = 1;}



    if(test_current->parents().size() == 1) { 
      test_current = test_current->parents()[0]; 
    }
    if(test_current->parents().size() == 2) { 
      //      test_current2 = test_current->parents()[0]; 

      test_current = test_current->parents()[1]; 
    } 
    

  }
  


  
  return istrue;
}

