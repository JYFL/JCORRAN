#ifndef ALIJBASEDATAMANAGER_H
#define ALIJBASEDATAMANAGER_H

class AliJBaseDataManager{
    public:
        AliJBaseDataManager( AliJEvent* fEvent;);
        virtual ~AliJBaseDataManager();
        virtual int LoadInputList(TString filename);
        virtual int LoadData();
        virtual Double_t LoadEvent( int ievt ); 
        virtual int GetNEvents();
        AliJEvent* GetJEvent();
        virtual bool IsGoodEvent();

    private:
        AliJEvent * fEvent;
        Long_t      fEventIndex;

};
#endif
