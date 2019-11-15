#========================================================================================

isEmpty( def_mod )
{
    def_mod = 1;

    isEmpty( def_not_print_pri_messages ): message( "----- CONFIG_READER appended -----" )
    isEmpty( config_reader_dir ): error( "config_reader_dir path is incorrect" )

    HEADERS += $$config_reader_dir/config.h
    SOURCES += $$config_reader_dir/config.cpp
}

#========================================================================================
