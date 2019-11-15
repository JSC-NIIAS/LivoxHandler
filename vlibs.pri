//=======================================================================================

isEmpty( def_vlibs )
{
    def_vlibs = 1;

    isEmpty( def_not_print_pri_messages ): message( "----- about to append VLIBS -----" )
    isEmpty( VLIBS_DIR ): error( "VLIBS_DIR path is incorrect" )

    include( $$VLIBS_DIR/vlog/vlog.pri )
    include( $$VLIBS_DIR/vlog/vfilelog.pri )
    include( $$VLIBS_DIR/vapplication/vapplication.pri )
    include( $$VLIBS_DIR/vzcm/vzcm.pri )
    include( $$VLIBS_DIR/vgit/vgit.pri )
    include( $$VLIBS_DIR/vgio/keyfile/vgio_keyfile.pri )

    isEmpty( def_not_print_pri_messages ): message( "----- VLIBS appended -----" )
}

//=======================================================================================
