//=======================================================================================

isEmpty( def_sdks )
{
    def_sdks = 1;

    isEmpty( def_not_print_pri_messages ): message( "----- about to append SDK -----" )
    isEmpty( SDK_DIR ): error( "SDK_DIR path is incorrect" )

    isEmpty( def_not_print_pri_messages ): message( "----- SDK appended -----" )
}

//=======================================================================================
