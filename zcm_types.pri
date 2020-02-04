//=======================================================================================

isEmpty( def_zcm_types )
{
    def_zcm_types = 1;

    isEmpty( def_not_print_pri_messages ): message( "----- about to append ZCM_TYPES -----" )
    isEmpty( ZCM_DIR ): error( "ZCM_DIR path is incorrect" )

    INCLUDEPATH += $$ZCM_DIR/service/cpp_types/
    INCLUDEPATH += $$ZCM_DIR/lidar_livox/cpp_types/

    isEmpty( def_not_print_pri_messages ): message( "----- ZCM_DIR appended -----" )
}

//=======================================================================================
