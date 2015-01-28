//==========================================================================

// A derived class for (e+ e- ->) GenericResonance -> various final states.

class Sigma1GenRes : public Sigma1Process {
    
public:
    
    // Constructor.
    Sigma1GenRes() {}
    
    // Evaluate sigmaHat(sHat): dummy unit cross section.
    virtual double sigmaHat() {return 1.;}
    
    // Select flavour. No colour or anticolour.
    virtual void setIdColAcol() {setId( -11, 11, 999999);
        setColAcol( 0, 0, 0, 0, 0, 0);}
    
    // Info on the subprocess.
    virtual string name()    const {return "GenericResonance";}
    virtual int    code()    const {return 9001;}
    virtual string inFlux()  const {return "ffbarSame";}
    
};

