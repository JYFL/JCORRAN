// -*- C++ -*-
#ifndef MYNAME_JCORRANherwig_H
#define MYNAME_JCORRANherwig_H
//
// This is the declaration of the JCORRANherwig class.
//

#include "ThePEG/Handlers/AnalysisHandler.h"
#include "ThePEG/EventRecord/Particle.h"
#include "ThePEG/EventRecord/Event.h"
#include "ThePEG/EventRecord/StandardSelectors.h"

#include <TStopwatch.h>
#include "JHerwigDataManager.h"
#include <AliJConst.h>

// jcorran main class
// used in local and grid execution
// blah

class AliJCard;
class AliJHistos;
class AliJCorrelations;
class AliJEventPool;
class AliJHistos;
class AliJEfficiency;
class JHerwigDataManager;
class AliJTrackCounter;

class TClonesArray;
class TF1;



namespace MyName {

	using namespace ThePEG;

	/**
	 * Here is the documentation of the JCORRANherwig class.
	 *
	 * @see \ref JCORRANherwigInterfaces "The interfaces"
	 * defined for JCORRANherwig.
	 */
	class JCORRANherwig: public AnalysisHandler {

		public:

			/** @name Standard constructors and destructors. */
			//@{
			/**
			 * The default constructor.
			 */
			JCORRANherwig();

			/**
			 * The destructor.
			 */
			virtual ~JCORRANherwig();
			//@}

		public:

			/** @name Virtual functions required by the AnalysisHandler class. */
			//@{
			/**
			 * Analyze a given Event. Note that a fully generated event
			 * may be presented several times, if it has been manipulated in
			 * between. The default version of this function will call transform
			 * to make a lorentz transformation of the whole event, then extract
			 * all final state particles and call analyze(tPVector) of this
			 * analysis object and those of all associated analysis objects. The
			 * default version will not, however, do anything on events which
			 * have not been fully generated, or have been manipulated in any
			 * way.
			 * @param event pointer to the Event to be analyzed.
			 * @param ieve the event number.
			 * @param loop the number of times this event has been presented.
			 * If negative the event is now fully generated.
			 * @param state a number different from zero if the event has been
			 * manipulated in some way since it was last presented.
			 */
			virtual void analyze(tEventPtr event, long ieve, int loop, int state);

			/**
			 * Transform the event to the desired Lorentz frame and return the
			 * corresponding LorentzRotation.
			 * @param event a pointer to the Event to be transformed.
			 * @return the LorentzRotation used in the transformation.
			 */
			virtual LorentzRotation transform(tEventPtr event) const;

			/**
			 * Analyze the given vector of particles. The default version calls
			 * analyze(tPPtr) for each of the particles.
			 * @param particles the vector of pointers to particles to be analyzed
			 */
			virtual void analyze(const tPVector & particles);

			/**
			 * Analyze the given particle.
			 * @param particle pointer to the particle to be analyzed.
			 */
			virtual void analyze(tPPtr particle);
			//@}

		public:

			/**
			 * The standard Init function used to initialize the interfaces.
			 * Called exactly once for each class by the class description system
			 * before the main function starts or
			 * when this class is dynamically loaded.
			 */
			static void Init();
			Int_t GetNumberEvents() const { return fnumberEvents; }
			AliJHistos *GetHistos() { return fhistos; }
			AliJCorrelations *GetCorrelations() { return fcorrelations; }
			AliJEventPool *GetAssocPool() { return fassocPool; }
			Int_t GetEventCounter() const { return fEventCounter; }
			AliJCard *GetCard() { return fcard; }
			void FillXtHistos(TClonesArray*, AliJTrackCounter *lPtrackCounter); // Esko

			void SetCard( AliJCard *c ) { fcard = c; }
			void SetTrigger( char* p ) { fjtrigg = GetParticleType(p); }
			void SetAssoc( char* p ) { fjassoc = GetParticleType(p); }
			void SetInputFile( char *f ) { finputFile = f; }
			void SetNumberEvents( Int_t n ) { fnumberEvents = n; }
			void SetRandomSeed( Int_t n ) { frandom_seed = n; }
			double DeltaPhi(double phi1, double phi2);
			particleType  GetParticleType(char *inchar);
			void ScaleNotEquidistantHisto(TH1D *hid, const double sc);


		protected:

			/** @name Clone Methods. */
			//@{
			/**
			 * Make a simple clone of this object.
			 * @return a pointer to the new object.
			 */
			virtual IBPtr clone() const;

			/** Make a clone of this object, possibly modifying the cloned object
			 * to make it sane.
			 * @return a pointer to the new object.
			 */
			virtual IBPtr fullclone() const;
			//@}


			// If needed, insert declarations of virtual function defined in the
			// InterfacedBase class here (using ThePEG-interfaced-decl in Emacs).

		protected:

			/** @name Standard Interfaced functions. */
			//@{
			/**
			 * Check sanity of the object during the setup phase.
			 */
			virtual void doupdate() throw(UpdateException);

			/**
			 * Initialize this object after the setup phase before saving an
			 * EventGenerator to disk.
			 * @throws InitException if object could not be initialized properly.
			 */
			virtual void doinit() throw(InitException);

			/**
			 * Initialize this object. Called in the run phase just before
			 * a run begins.
			 */
			virtual void doinitrun();

			/**
			 * Finalize this object. Called in the run phase just after a
			 * run has ended. Used eg. to write out statistics.
			 */
			virtual void dofinish();

			/**
			 * Rebind pointer to other Interfaced objects. Called in the setup phase
			 * after all objects used in an EventGenerator has been cloned so that
			 * the pointers will refer to the cloned objects afterwards.
			 * @param trans a TranslationMap relating the original objects to
			 * their respective clones.
			 * @throws RebindException if no cloned object was found for a given
			 * pointer.
			 */
			virtual void rebind(const TranslationMap & trans) throw(RebindException);

			/**
			 * Return a vector of all pointers to Interfaced objects used in this
			 * object.
			 * @return a vector of pointers.
			 */
			virtual IVector getReferences();
			//@}


		private:

			/**
			 * The static object used to initialize the description of this class.
			 * Indicates that this is an concrete class without persistent data.
			 */
			static NoPIOClassDescription<JCORRANherwig> initJCORRANherwig;

			/**
			 * The assignment operator is private and must never be called.
			 * In fact, it should not even be implemented.
			 */
			JCORRANherwig & operator=(const JCORRANherwig &);

		private:

			/**
			 * A pointer to a Root histogram
			 */
			EventPtr event;
			TStopwatch timer;
            TFile *fout;
			Bool_t fExecLocal; // exec mode
			Bool_t fFirstEvent; //!

			particleType fjtrigg; // assoc
			particleType fjassoc; // trigger

			AliJCard *fcard; // card
			char * finputFile; //!

			Int_t fevt; // event number
			AliJHistos *fhistos; //!
			AliJCorrelations *fcorrelations; //!
			AliJEventPool *fassocPool; //! 
			TClonesArray *fchargedHadronList; //!
			TClonesArray *ftriggList; //! 
			TClonesArray *fassocList; //!
			TClonesArray *fpairList; //!
			TClonesArray *fpairCounterList; //!
			TClonesArray *finputList; //!

			JHerwigDataManager *fdmg;

			int fnumberEvents; //!
			int fieout; //!
			int fEventCounter; //!
			double fcent; //!
			Int_t fncBin; //!
			Int_t fnPttBin; //!
			bool fbTriggCorrel; //!
			bool fbLPCorrel; //!
			bool fbLPpairCorrel; //!
			double fTrackEtaRange; //!
			double flowerPtAssocBoarder; //!

			AliJEfficiency *fEfficiency;
			TNamed *fpythiaInfo;
			Int_t frandom_seed;
			TString fPythiaConfig;
			TString fPythiaSettingAll;
			float fIsolationR;
			Float_t fSQRTS;


	};

}

#include "ThePEG/Utilities/ClassTraits.h"

namespace ThePEG {

	/** @cond TRAITSPECIALIZATIONS */

	/** This template specialization informs ThePEG about the
	 *  base classes of JCORRANherwig. */
	template <>
		struct BaseClassTrait<MyName::JCORRANherwig,1> {
			/** Typedef of the first base class of JCORRANherwig. */
			typedef AnalysisHandler NthBase;
		};

	/** This template specialization informs ThePEG about the name of
	 *  the JCORRANherwig class and the shared object where it is defined. */
	template <>
		struct ClassTraits<MyName::JCORRANherwig>
		: public ClassTraitsBase<MyName::JCORRANherwig> {
			/** Return a platform-independent class name */
			static string className() { return "MyName::JCORRANherwig"; }
			/**
			 * The name of a file containing the dynamic library where the class
			 * JCORRANherwig is implemented. It may also include several, space-separated,
			 * libraries if the class JCORRANherwig depends on other classes (base classes
			 * excepted). In this case the listed libraries will be dynamically
			 * linked in the order they are specified.
			 */
			static string library() { return "libJCORRANherwig.so"; }
		};

	/** @endcond */

}

#endif /* MYNAME_JCORRANherwig_H */
