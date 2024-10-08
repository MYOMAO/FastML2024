//--------------------------------------------------------------------------
#ifndef HEPMC_PARTICLE_DATA_TABLE_H
#define HEPMC_PARTICLE_DATA_TABLE_H

// Matt.Dobbs@Cern.CH, Jan 2000, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics", Computer Physics Communications (to be published).
//
// Container for GenParticle Data Instances --- basically just an interface
//   to STL map -- the same naming conventions are used
// A GenParticle may belong to any number of ParticleDataTables. 
// The ParticleDataTable does not own the ParticleData objects and will NOT
//   delete them unless explicity told to do so with the delete_all method.
// Each ParticleData entry in the table MUST have a unique pdg_id (otherwise
//   an attempt to insert it as a new entry will fail).
// Updated 2000.02.08 M.Dobbs added merge_table and 
//                            make_antiparticles_from_particles

#include <iostream>
#include <map>
#include <cstdio>       // needed for formatted output using sprintf 
#include "ParticleData.h"

namespace HepMC {

    
    class ParticleDataTable {
  
    public:
        ParticleDataTable( std::string description = std::string() );
        ParticleDataTable( const char description );
        ParticleDataTable( const ParticleDataTable& );
        virtual ~ParticleDataTable(); 
        ParticleDataTable& operator=( const ParticleDataTable& );

        void    make_antiparticles_from_particles();
        int     merge_table( const ParticleDataTable& );

        void    print( std::ostream& ostr = std::cout ) const;

        void    delete_all(); 
        void    clear();      

        ParticleData*       operator[]( int id ) const;
        ParticleData*       find( int id ) const;
        int                 size() const;
        bool                empty() const;
        bool                insert( ParticleData* ); 
        bool                erase( ParticleData* );  
        bool                erase( int id );       
        typedef std::map<int,HepMC::ParticleData*>::iterator iterator;
        typedef std::map<int,HepMC::ParticleData*>::const_iterator const_iterator;
        iterator            begin();
        iterator            end();
        const_iterator      begin() const;
        const_iterator      end() const;
        
        // access methods //
        
        std::string description() const;
        void        set_description( std::string );
        void        set_description( const char );

    private:
        std::string                        m_description;
        std::map<int,HepMC::ParticleData*> m_data_table;
    };

    // INLINES               //

    inline ParticleDataTable::ParticleDataTable( std::string description ) 
        : m_description(description) {
        std::cout << "-------------------------------------------------------" << std::endl;
        std::cout << "Use of HepMC/ParticleDataTable is deprecated" << std::endl;
        std::cout << "-------------------------------------------------------" << std::endl;
    }

    inline ParticleDataTable::ParticleDataTable( const char description ) {
        m_description = description; 

        std::cout << "-------------------------------------------------------" << std::endl;
        std::cout << "Use of HepMC/ParticleDataTable is deprecated" << std::endl;
        std::cout << "-------------------------------------------------------" << std::endl;
    }

    inline ParticleDataTable::ParticleDataTable( const ParticleDataTable& pdt){
        *this = pdt;
    }

    inline ParticleDataTable::~ParticleDataTable(){}

    inline ParticleDataTable& ParticleDataTable::operator=( const 
                                                            ParticleDataTable& 
                                                            pdt) {
        m_description = pdt.m_description;
        m_data_table = pdt.m_data_table;
        return *this;
    }

    inline void ParticleDataTable::make_antiparticles_from_particles() {
        ParticleDataTable new_data;
        for ( ParticleDataTable::iterator p = begin(); p != end(); ++p ) {
            ParticleData* pdata = p->second;
            if ( pdata->charge() ) {
                new_data.insert( new ParticleData( pdata->name()+"~",
                                                   -1*pdata->pdg_id(),
                                                   -1.*pdata->charge(), 
                                                   pdata->mass(),
                                                   pdata->clifetime(), 
                                                   pdata->spin() ));
            }
        }
        merge_table( new_data );
    }

    inline void ParticleDataTable::print( std::ostream& ostr ) const {
        //
        ostr << "________________________________________"
             << "________________________________________\n";
        ostr << "ParticleData:   *****  ParticleDataTable"
             << "  *****   ( " << size() 
             << " entries )\n";
        ostr << " Description: " << m_description << "\n";
        ostr << "   PDG ID " << "       PARTICLE NAME "
             << "CHARGE" <<    "     MASS     "
             << "  C*LIFETIME (CM) " << " SPIN\n";
        for ( std::map< int,ParticleData* >::const_iterator pd 
                  = m_data_table.begin(); pd != m_data_table.end(); pd++ ) {
            ostr << *(pd->second) << "\n";
        }
        ostr << "________________________________________"
             << "________________________________________" << std::endl;
    }      

    inline ParticleData* ParticleDataTable::find( int id ) const {
        std::map<int,ParticleData*>::const_iterator iter 
            = m_data_table.find(id);
        return ( iter == m_data_table.end() ) ? 0 : iter->second;
    }

    inline ParticleData* ParticleDataTable::operator[]( int id ) const {
        return find(id);
    }

    inline int ParticleDataTable::size() const { 
        return (int)m_data_table.size();
    }

    inline bool ParticleDataTable::empty() const {
        return (bool)m_data_table.empty();
    }

    inline bool ParticleDataTable::insert( ParticleData* pdata ) {
        if ( m_data_table.count(pdata->pdg_id()) ) return 0;
        return ( m_data_table[pdata->pdg_id()] = pdata ); // true is success
    }

    inline bool ParticleDataTable::erase( ParticleData* pdata ) {
        return (bool)m_data_table.erase( pdata->pdg_id() );
    }


    inline bool ParticleDataTable::erase( int id ) {
        return (bool)m_data_table.erase( id );
    }

    inline ParticleDataTable::iterator ParticleDataTable::begin() { 
        return m_data_table.begin(); 
    }

    inline ParticleDataTable::iterator ParticleDataTable::end() { 
        return m_data_table.end(); 
    }

    inline ParticleDataTable::const_iterator ParticleDataTable::begin() const {
        return m_data_table.begin(); 
    }

    inline ParticleDataTable::const_iterator ParticleDataTable::end() const { 
        return m_data_table.end(); 
    }

    inline std::string ParticleDataTable::description() const { 
        return m_description; 
    }

    inline void ParticleDataTable::set_description( std::string description ) {
        m_description = description;
    }

    inline void ParticleDataTable::set_description( const char description ) {
        m_description = description;
    }

    inline void ParticleDataTable::delete_all() {
        for ( std::map<int,ParticleData*>::iterator pd = m_data_table.begin();
              pd != m_data_table.end(); pd++) delete pd->second;
        clear();
    }

    inline void ParticleDataTable::clear() { m_data_table.clear(); }

    inline int ParticleDataTable::merge_table( const ParticleDataTable& pdt ) {
        int count_number_insertions =0;
        for ( ParticleDataTable::const_iterator p = pdt.begin(); 
              p != pdt.end(); ++p ) {
            if ( insert(p->second) ) ++count_number_insertions;
        }
        return count_number_insertions;
    }

} // HepMC

#endif  // HEPMC_PARTICLE_DATA_TABLE_H
//--------------------------------------------------------------------------

