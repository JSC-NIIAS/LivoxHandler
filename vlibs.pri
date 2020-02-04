//=======================================================================================

isEmpty( def_vlibs )
{
    def_vlibs = 1;

    isEmpty( def_not_print_pri_messages ): message( "----- about to append VLIBS -----" )
    isEmpty( VLIBS_DIR ): error( "VLIBS_DIR path is incorrect" )

    include( $$VLIBS_DIR/qmake/vlog.pri )
    include( $$VLIBS_DIR/qmake/vbyte_buffer.pri )
    include( $$VLIBS_DIR/qmake/vchrono.pri )

    isEmpty( def_not_print_pri_messages ): message( "----- VLIBS appended -----" )
}

//=======================================================================================
