//--------------------------------------------------------------------------
#ifndef HEPMC_PARTICLE_DATA_H
#define HEPMC_PARTICLE_DATA_H

// Matt.Dobbs@Cern.CH, September 1999, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics", Computer Physics Communications (to be published).
//
// GenParticle Data common to all particles of a given PDG id
//
// Units  ID: defined by PDG group (particles are +ve, antiparticles are -ve) 
//              also consistent with the Pythia definitions
//              See: http://d0lblt.lbl.gov/wwwpdg/mc_numbers.htm
//        charge: fraction of proton charge
//        mass in user defined energy units
//        width:  ( stored as cLifetime = hbar / Width )
//        cLifetime: c*time
//        spin: fraction of photon spin (always a positive number)
//
// Default mass is 0.
// Default cLifetime is -1 which means stable (setting width = 0 gives this)
//     (we define cLifetime = -1 --> width = 0 (i.e stable), 
//                    width = -1 --> cLifetime = 0 (i.e. prompt)  )
// These defaults exist because many very basic MC generators
//     may produce only massless stable particles in the event record.
//
// It is intended that a different ParticleData object is created for each 
//     particle and its anti-particle - useful for CP violation studies.
//
// There are few set methods for this class, there should be no reason
//     to change anything after instantiating. If you need to, then
//     create a new object and kill the old one.
//
// Example: 
//  HepMC::ParticleData* pd_electron = 
//                    new HepMC::ParticleData("electron",11,-1,0.000511,-1,.5);
// A method is provided to allow you to set the lifetime from the
//     width in the constructor
// Example: new HepMC::ParticleData("W+",24,+1,80.396,
//                                  HepMC::clifetime_from_width(2.06),1);
// Example of finding a ParticleData object from its PDG ID 
// in ParticleDataTable pdt:
//        HepMC::ParticleData* electron = pdt.find(11);
// or if you just wanted two know the electron mass, you could do:
//        pdt.find(11)->mass();

#include <iostream>
#include <string>
#include <stdlib.h>   // for integer abs()

namespace HepMC {

    static const double HepMC_hbarc   = (6.6260755e-34 * (1.e-6/1.60217733e-19) / (2*3.14159265358979323846))
                                        * (2.99792458e+8 * 1000.) * 1.e+3;

    // if you want to instantiate the particle lifetime from its width,
    // use this static method inside the constructor:
    double clifetime_from_width( double width );  

    
    class ParticleData {
  
        friend std::ostream&  operator<<( std::ostream&, const ParticleData& );

    public:
        ParticleData( std::string name, int id, double charge, double mass = 0,
                      double cLifetime = -1, double spin = 0 );
        ParticleData( const char* name, int id, double charge, double mass = 0,
                      double cLifetime = -1, double spin = 0 );
        virtual ~ParticleData();

        bool    operator==( const ParticleData& ) const; 
        bool    operator!=( const ParticleData& ) const; 

        void    print( std::ostream& ostr = std::cout ) const;

        bool    is_lepton() const;        
        bool    is_charged_lepton() const;
        bool    is_em() const;            
        bool    is_neutrino() const;      
        bool    is_hadron() const;        
        bool    is_boson() const;         

        // access methods //
        
        std::string  name() const;
        int          pdg_id() const;
        double       charge() const;
        double       mass() const;
        double       width() const; 
        double       clifetime() const;
        double       spin() const;

        void         set_charge( double ); 
        void         set_mass( double ); 
        void         set_width( double ); 
        void         set_clifetime( double ); 
        void         set_spin( double ); 

    protected:
        static unsigned int counter(); 
    
        int    model_independent_pdg_id_() const;

    private:
        std::string   m_name;   // description of the particle according to PDG
                                // i.e. "Delta(1900) S_31"
        int           m_pdg_id; // PDG ID number (note we allow -ve)
        int           m_3charge;// 3*electric charge in units of proton charge
        double        m_mass;   // nominal mass in user defined energy units
        double        m_clifetime; // [mm]
        unsigned char m_2spin;  // 2*spin (J) of particle

        static unsigned int s_counter;
    };

    // INLINES               //

    inline bool ParticleData::is_lepton() const {
        return ( abs(pdg_id()) >=11 &&  abs(pdg_id()) <= 18 );
    }
    inline bool ParticleData::is_charged_lepton() const {
        return ( is_lepton() && abs(pdg_id())%2==1 );
    }
    inline bool ParticleData::is_neutrino() const {
        return ( is_lepton() && abs(pdg_id())%2==0 );
    }
    inline bool ParticleData::is_em() const {
        return ( abs(pdg_id()) == 11 || abs(pdg_id()) == 22 );
    }
    inline bool ParticleData::is_hadron() const {
        return ( abs(pdg_id()) <= 9 || abs(pdg_id()) == 21 
                 || abs(pdg_id()) >100 );
    }
    inline bool ParticleData::is_boson() const {
        return ( ( abs(pdg_id()) >20 && abs(pdg_id()) <=40 )
                 || abs(pdg_id()) == 9 );
    }

    // INLINE Access Methods //

    inline std::string ParticleData::name() const { return m_name; }
    inline int         ParticleData::pdg_id() const { return m_pdg_id; }
    inline double      ParticleData::charge() const { 
        return ( (double)m_3charge )/3.; 
    }
    inline double      ParticleData::mass() const { return m_mass; }
    inline double      ParticleData::clifetime() const { return m_clifetime; }
    inline double      ParticleData::spin() const { return m_2spin/2.; }
    inline void        ParticleData::set_charge( double charge ) {
        if ( charge > 0 ) { 
            m_3charge = (int)(3.*charge+.1); 
        } else if ( charge < 0. ) { 
            m_3charge = (int)(3.*charge-.1); 
        } else { 
            m_3charge = 0; 
        }
    }
    inline void        ParticleData::set_mass( double its_mass ) {
        m_mass = its_mass;
    }
    inline void        ParticleData::set_width( double width ) {
        if ( width > 0 ) { 
            m_clifetime = HepMC_hbarc/width;
        } else if ( width == 0. ) { 
            m_clifetime = -1.;
        } else { 
            m_clifetime = 0.; 
        }
    }
    inline void        ParticleData::set_clifetime( double its_clifetime ) {
        m_clifetime = its_clifetime; 
    }
    inline void        ParticleData::set_spin( double spin ) {
        m_2spin = (unsigned char)(2.*spin+.1);
    }

    // INLINE Operators      //

    inline bool ParticleData::operator==( const ParticleData& a ) const {
        // compares everything except the particle's name
        return ( a.m_pdg_id != m_pdg_id ||
                 a.m_mass != m_mass ||
                 a.m_clifetime != m_clifetime ||
                 a.m_3charge != m_3charge ||
                 a.m_2spin != m_2spin            ) ? 0 : 1;
    }

    inline bool ParticleData::operator!=( const ParticleData& a ) const {
        // compares everything except the particle's name
        return ( a.pdg_id() != this->pdg_id() );
    }

} // HepMC

#endif  // HEPMC_PARTICLE_DATA_H
//--------------------------------------------------------------------------

