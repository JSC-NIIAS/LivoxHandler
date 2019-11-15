#include "config.h"

#include "vlog.h"

//=======================================================================================
Config Config::load( const std::string& fname )
{
    Config conf;
    Config::Schema schema( &conf );

    try
    {
        auto kf = vgio::KeyFile::from_file( fname );
        schema.schema.capture( kf );
    }
    catch ( const vgio::Exception & )
    {
        vfatal.nospace()( "Cannot read configuration from file '", fname, "'." );
        throw;
    }

    return conf;
}
//=======================================================================================
std::string Config::get()
{
    Config conf;
    return Config::Schema( &conf ).schema.build( keyfile_heap ).to_data();
}
//=======================================================================================
