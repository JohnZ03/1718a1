verdiWindowResize -win $_Verdi_1 "65" "24" "1855" "1056"
debImport "-f" "filelist.f"
debLoadSimResult \
           /home/wugxfly/Documents/cnn_mnist_1718/1718a1/maxpooling/maxpooling.fsdb
wvCreateWindow
srcHBSelect "maxpooling_tb.dut" -win $_nTrace1
srcHBSelect "maxpooling_tb.dut" -win $_nTrace1
srcSetScope -win $_nTrace1 "maxpooling_tb.dut" -delim "."
srcDeselectAll -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "clk" -win $_nTrace1
srcSelect -signal "rst_n" -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "ena" -win $_nTrace1
srcSelect -signal "rst_n" -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "clk" -win $_nTrace1
srcSelect -signal "rst_n" -win $_nTrace1
srcSelect -signal "ena" -win $_nTrace1
srcSelect -signal "frame_start_in" -win $_nTrace1
srcSelect -signal "line_start_in" -win $_nTrace1
srcSelect -toggle -signal "line_start_in" -win $_nTrace1
srcSelect -signal "line_start_in" -win $_nTrace1
srcSelect -signal "frame_end_in" -win $_nTrace1
srcSelect -signal "sig_layer" -win $_nTrace1
srcSelect -signal "max_layer" -win $_nTrace1
srcSelect -signal "frame_start_out" -win $_nTrace1
srcSelect -signal "line_start_out" -win $_nTrace1
srcSelect -signal "frame_end_out" -win $_nTrace1
srcSelect -signal "valid" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "frame_start_in" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "next_state" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "state" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "frame_end_in" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvSetCursor -win $_nWave2 7865486.725664 -snap {("G1" 16)}
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 7986788.874842 -snap {("G1" 15)}
wvSetCursor -win $_nWave2 7833565.107459 -snap {("G1" 16)}
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoom -win $_nWave2 7303666.245259 8376232.616941
wvSetCursor -win $_nWave2 7878594.363581 -snap {("G1" 11)}
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 86781.602766 -snap {("G1" 5)}
wvZoomIn -win $_nWave2
wvSetCursor -win $_nWave2 7842887.349943 -snap {("G1" 5)}
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
srcHBSelect "maxpooling_tb" -win $_nTrace1
srcHBSelect "maxpooling_tb" -win $_nTrace1
srcHBSelect "maxpooling_tb" -win $_nTrace1
srcSetScope -win $_nTrace1 "maxpooling_tb" -delim "."
srcDeselectAll -win $_nTrace1
srcSelect -signal "frame_start_in" -win $_nTrace1
srcSearchString "frame_start_in" -win $_nTrace1 -next -case
srcSearchString "frame_start_in" -win $_nTrace1 -next -case
srcSearchString "frame_start_in" -win $_nTrace1 -next -case
srcSearchString "frame_start_in" -win $_nTrace1 -next -case
srcAction -pos 48 4 0 -win $_nTrace1 -name " " -ctrlKey off
srcAction -pos 48 4 0 -win $_nTrace1 -name " " -ctrlKey off
debReload
wvZoom -win $_nWave2 7814683.329043 7898753.006723
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 7851595.337966 -snap {("G1" 13)}
wvZoomIn -win $_nWave2
wvZoom -win $_nWave2 7817584.848892 7902611.071577
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvSetCursor -win $_nWave2 7553960.561745 -snap {("G1" 14)}
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 261420.699834 -snap {("G1" 14)}
wvZoomIn -win $_nWave2
wvSetCursor -win $_nWave2 7264743.658537 -snap {("G1" 13)}
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoom -win $_nWave2 7078997.371813 7498646.389967
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvSetCursor -win $_nWave2 6393287.692970 -snap {("G1" 3)}
wvZoomIn -win $_nWave2
wvSetCursor -win $_nWave2 305585.125736 -snap {("G1" 3)}
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 596845.948702 -snap {("G1" 3)}
debReload
srcHBSelect "maxpooling_tb" -win $_nTrace1
srcSetScope -win $_nTrace1 "maxpooling_tb" -delim "."
wvDisplayGridCount -win $_nWave2 -off
wvGetSignalClose -win $_nWave2
wvReloadFile -win $_nWave2
wvDisplayGridCount -win $_nWave2 -off
wvGetSignalClose -win $_nWave2
wvReloadFile -win $_nWave2
wvDisplayGridCount -win $_nWave2 -off
wvGetSignalClose -win $_nWave2
wvReloadFile -win $_nWave2
wvDisplayGridCount -win $_nWave2 -off
wvGetSignalClose -win $_nWave2
wvReloadFile -win $_nWave2
debReload
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 8363463.969659 -snap {("G1" 4)}
wvSetCursor -win $_nWave2 8427307.206068 -snap {("G1" 3)}
debReload
wvSetCursor -win $_nWave2 7980404.551201 -snap {("G1" 16)}
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoom -win $_nWave2 7827680.877812 7886941.307647
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 191529.709229 -snap {("G1" 3)}
debReload
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvDisplayGridCount -win $_nWave2 -off
wvGetSignalClose -win $_nWave2
wvReloadFile -win $_nWave2
debReload
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 291363.780025 -snap {("G1" 5)}
wvZoomIn -win $_nWave2
wvSetCursor -win $_nWave2 583715.233881 -snap {("G1" 6)}
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 7822376.738306 -snap {("G1" 6)}
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoom -win $_nWave2 7680151.706700 8051517.067004
wvSetCursor -win $_nWave2 7864895.409961 -snap {("G1" 11)}
wvSelectSignal -win $_nWave2 {( "G1" 16 )} 
wvCut -win $_nWave2
wvSetPosition -win $_nWave2 {("G2" 0)}
wvSetPosition -win $_nWave2 {("G1" 15)}
srcSetScope -win $_nTrace1 "maxpooling_tb.dut" -delim "."
srcHBSelect "maxpooling_tb.dut" -win $_nTrace1
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 300472.605050 -snap {("G1" 9)}
debReload
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
srcHBSelect "maxpooling_tb.dut.u_sram" -win $_nTrace1
srcHBSelect "maxpooling_tb.dut.u_sram" -win $_nTrace1
srcSetScope -win $_nTrace1 "maxpooling_tb.dut.u_sram" -delim "."
srcSelect -signal "clk" -win $_nTrace1
srcSelect -signal "en_n" -win $_nTrace1
srcSelect -signal "wren_n" -win $_nTrace1
srcSelect -signal "addr" -win $_nTrace1
srcSelect -signal "data_i" -win $_nTrace1
srcSelect -signal "data_o" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 1562457.546258 -snap {("G1" 7)}
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 4800049.865665 -snap {("G1" 20)}
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
srcHBSelect "maxpooling_tb.dut" -win $_nTrace1
srcSetScope -win $_nTrace1 "maxpooling_tb.dut" -delim "."
srcDeselectAll -win $_nTrace1
srcSelect -signal "sram0_rdata" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
srcHBSelect "maxpooling_tb.dut.u_max4" -win $_nTrace1
srcHBSelect "maxpooling_tb.dut.u_max4" -win $_nTrace1
srcSetScope -win $_nTrace1 "maxpooling_tb.dut.u_max4" -delim "."
srcSelect -signal "i_0" -win $_nTrace1
srcSelect -signal "i_1" -win $_nTrace1
srcSelect -signal "i_2" -win $_nTrace1
srcSelect -signal "i_3" -win $_nTrace1
srcSelect -signal "o_0" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 0
srcHBSelect "maxpooling_tb.dut" -win $_nTrace1
srcSetScope -win $_nTrace1 "maxpooling_tb.dut" -delim "."
srcDeselectAll -win $_nTrace1
srcSelect -signal "max_value" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvSelectSignal -win $_nWave2 {( "G1" 1 )} 
wvSelectAll -win $_nWave2
srcDeselectAll -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "frame_end_in" -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "sig_layer" -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "max_layer" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvScrollDown -win $_nWave2 1
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
verdiDockWidgetHide -dock windowDock_nWave_2
verdiDockWidgetHide -dock widgetDock_<Message>
srcDeselectAll -win $_nTrace1
srcSelect -signal "line_start_in" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 0
srcDeselectAll -win $_nTrace1
srcSelect -signal "sram0_addr0_dly" -win $_nTrace1
srcSelect -signal "state_read_dly" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "sram0_addr0_dly" -win $_nTrace1
debReload
wvUnknownSaveResult -win $_nWave2 -clear
wvSetCursor -win $_nWave2 270425.344545 -snap {("G1" 21)}
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvScrollUp -win $_nWave2 5
wvSelectSignal -win $_nWave2 {( "G1" 1 )} 
wvSelectSignal -win $_nWave2 {( "G1" 1 2 )} 
wvSelectAll -win $_nWave2
wvCut -win $_nWave2
wvSetPosition -win $_nWave2 {("G2" 0)}
srcSelect -signal "clk" -win $_nTrace1
srcSelect -signal "rst_n" -win $_nTrace1
srcSelect -signal "ena" -win $_nTrace1
srcSelect -signal "frame_start_in" -win $_nTrace1
srcSelect -signal "line_start_in" -win $_nTrace1
srcSelect -signal "frame_end_in" -win $_nTrace1
srcSelect -signal "sig_layer" -win $_nTrace1
srcSelect -signal "max_layer" -win $_nTrace1
srcSelect -signal "frame_start_out" -win $_nTrace1
srcSelect -signal "line_start_out" -win $_nTrace1
srcSelect -signal "frame_end_out" -win $_nTrace1
srcSelect -signal "valid" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvSetCursor -win $_nWave2 304228.512613 -snap {("G2" 7)}
srcDeselectAll -win $_nTrace1
srcSelect -signal "sram0_addr_dly" -win $_nTrace1
srcSelect -signal "state_read_dly" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
debReload
wvUnknownSaveResult -win $_nWave2 -clear
wvUnknownSaveResult -win $_nWave2 -clear
srcDeselectAll -win $_nTrace1
srcSelect -signal "state_read_dly" -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "sram0_addr_dly" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvSetCursor -win $_nWave2 314087.769966 -snap {("G3" 0)}
wvSetCursor -win $_nWave2 328172.423328 -snap {("G3" 1)}
wvSetCursor -win $_nWave2 335684.238454 -snap {("G2" 8)}
srcDeselectAll -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "sig_layer_dly1" -win $_nTrace1
srcTBBTreeSelect -win $_nTrace1 -path "max4"
srcTBBTreeSelect -win $_nTrace1 -path "max4"
srcTBBTreeSelect -win $_nTrace1 -path "max4"
srcTBTreeAction -win $_nTrace1 -path "max4"
srcDeselectAll -win $_nTrace1
srcSelect -signal "i_0" -win $_nTrace1
srcSelect -signal "i_1" -win $_nTrace1
srcSelect -signal "i_2" -win $_nTrace1
srcSelect -signal "i_3" -win $_nTrace1
srcSelect -signal "o_0" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
srcTBBTreeSelect -win $_nTrace1 -path "maxpooling"
srcTBBTreeSelect -win $_nTrace1 -path "maxpooling"
srcTBTreeAction -win $_nTrace1 -path "maxpooling"
srcDeselectAll -win $_nTrace1
srcSelect -signal "sram0_rdata_dly1" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "sram0_rdata" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvSetCursor -win $_nWave2 323477.538874 -snap {("G2" 7)}
wvSetCursor -win $_nWave2 312209.816184 -snap {("G2" 7)}
wvSetCursor -win $_nWave2 306575.954840 -snap {("G2" 7)}
wvSetCursor -win $_nWave2 314557.258411 -snap {("G2" 7)}
srcDeselectAll -win $_nTrace1
srcSelect -signal "sram0_wdata" -win $_nTrace1
srcSelect -signal "sram0_rdata" -win $_nTrace1
srcSelect -signal "sram0_addr" -win $_nTrace1
srcSelect -signal "sram0_wen" -win $_nTrace1
srcSelect -signal "sram0_en" -win $_nTrace1
srcSelect -signal "clk" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
srcTBBTreeSelect -win $_nTrace1 -path "sram0"
srcTBBTreeSelect -win $_nTrace1 -path "sram0"
srcTBTreeAction -win $_nTrace1 -path "sram0"
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 0
wvSetCursor -win $_nWave2 35681.121850 -snap {("G3" 12)}
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvSelectGroup -win $_nWave2 {G3}
wvSelectSignal -win $_nWave2 {( "G3" 1 )} 
wvSelectSignal -win $_nWave2 {( "G3" 1 2 )} 
wvSelectSignal -win $_nWave2 {( "G3" 1 2 4 )} 
wvSelectSignal -win $_nWave2 {( "G3" 1 2 4 5 )} 
wvSelectSignal -win $_nWave2 {( "G3" 1 2 4 5 6 )} 
wvSelectSignal -win $_nWave2 {( "G3" 1 2 3 4 5 6 )} 
wvSelectSignal -win $_nWave2 {( "G3" 1 2 3 4 5 6 7 )} 
wvSelectSignal -win $_nWave2 {( "G3" 1 2 3 4 5 6 7 8 )} 
wvSelectSignal -win $_nWave2 {( "G3" 1 2 3 4 5 6 7 8 9 )} 
wvSelectSignal -win $_nWave2 {( "G3" 1 2 3 4 5 6 7 8 9 10 )} 
wvCut -win $_nWave2
wvSetPosition -win $_nWave2 {("G3" 4)}
wvSetCursor -win $_nWave2 29108.283614 -snap {("G2" 7)}
wvSetCursor -win $_nWave2 35211.633404 -snap {("G2" 7)}
wvSetCursor -win $_nWave2 42253.960085 -snap {("G3" 1)}
srcDeselectAll -win $_nTrace1
srcSelect -signal "register" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvSetCursor -win $_nWave2 25352.376051 -snap {("G2" 6)}
wvSetCursor -win $_nWave2 24413.399160 -snap {("G2" 7)}
wvSetCursor -win $_nWave2 14554.141807 -snap {("G2" 1)}
wvSetCursor -win $_nWave2 23943.910715 -snap {("G2" 4)}
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoom -win $_nWave2 0.000000 30047.260505
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
srcTBBTreeSelect -win $_nTrace1 -path "maxpooling"
srcTBBTreeSelect -win $_nTrace1 -path "maxpooling"
srcTBTreeAction -win $_nTrace1 -path "maxpooling"
srcDeselectAll -win $_nTrace1
srcSelect -signal "state" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 0
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 24311.310649 -snap {("G3" 6)}
srcDeselectAll -win $_nTrace1
debReload
wvSetCursor -win $_nWave2 26742.441714 -snap {("G2" 7)}
wvSetCursor -win $_nWave2 36466.965973 -snap {("G3" 3)}
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvSetCursor -win $_nWave2 21880.179584 -snap {("G3" 2)}
wvSetCursor -win $_nWave2 863051.528031 -snap {("G2" 9)}
wvSetCursor -win $_nWave2 32820.269376 -snap {("G3" 3)}
wvSetCursor -win $_nWave2 1217996.663503 -snap {("G3" 5)}
wvDisplayGridCount -win $_nWave2 -off
wvGetSignalClose -win $_nWave2
wvReloadFile -win $_nWave2
wvSetCursor -win $_nWave2 305106.948642 -snap {("G2" 5)}
wvSetCursor -win $_nWave2 25526.876181 -snap {("G3" 2)}
wvZoomIn -win $_nWave2
srcDeselectAll -win $_nTrace1
srcSelect -signal "sram0_addr" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvSetCursor -win $_nWave2 35251.400441 -snap {("G3" 7)}
srcTBBTreeSelect -win $_nTrace1 -path "sram0"
srcTBBTreeSelect -win $_nTrace1 -path "sram0"
srcTBBTreeSelect -win $_nTrace1 -path "sram0"
srcTBTreeAction -win $_nTrace1 -path "sram0"
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvSetCursor -win $_nWave2 21272.396818 -snap {("G2" 7)}
wvSetCursor -win $_nWave2 31604.703843 -snap {("G3" 7)}
wvSetCursor -win $_nWave2 20056.831285 -snap {("G3" 6)}
srcTBBTreeSelect -win $_nTrace1 -path "maxpooling_tb"
srcTBBTreeSelect -win $_nTrace1 -path "maxpooling_tb"
srcTBTreeAction -win $_nTrace1 -path "maxpooling_tb"
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 0
wvSetCursor -win $_nWave2 35859.183207 -snap {("G3" 7)}
srcTBBTreeSelect -win $_nTrace1 -path "maxpooling"
srcTBBTreeSelect -win $_nTrace1 -path "maxpooling"
srcTBTreeAction -win $_nTrace1 -path "maxpooling"
srcDeselectAll -win $_nTrace1
srcSelect -signal "sram0_wdata" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
debReload
wvSetCursor -win $_nWave2 35251.400441 -snap {("G3" 7)}
wvSetCursor -win $_nWave2 41937.010869 -snap {("G3" 7)}
wvSetCursor -win $_nWave2 35859.183207 -snap {("G3" 7)}
wvZoom -win $_nWave2 24919.093415 40113.662570
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 0
wvSetCursor -win $_nWave2 25817.321043 -snap {("G3" 2)}
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvSetCursor -win $_nWave2 34730.443784 -snap {("G2" 7)}
wvScrollDown -win $_nWave2 1
wvScrollDown -win $_nWave2 0
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 36497.700879 -snap {("G3" 4)}
wvZoom -win $_nWave2 36151.933187 37150.817632
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 24892.553960 -snap {("G3" 6)}
wvSetCursor -win $_nWave2 35237.511449 -snap {("G3" 7)}
debReload
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvSetCursor -win $_nWave2 24892.553960 -snap {("G3" 8)}
wvSetCursor -win $_nWave2 35560.791371 -snap {("G3" 7)}
srcDeselectAll -win $_nTrace1
srcSelect -signal "sram0_rdata" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
srcSelect -signal "sig_layer_dly1" -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcTBBTreeSelect -win $_nTrace1 -path "maxpooling"
srcTBBTreeSelect -win $_nTrace1 -path "max4"
srcTBTreeAction -win $_nTrace1 -path "max4"
srcTBBTreeSelect -win $_nTrace1 -path "max4"
srcSelect -signal "i_0" -win $_nTrace1
srcSelect -signal "i_1" -win $_nTrace1
srcSelect -signal "i_2" -win $_nTrace1
srcSelect -signal "i_3" -win $_nTrace1
srcSelect -signal "o_0" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollDown -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollUp -win $_nWave2 1
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 0
wvSelectSignal -win $_nWave2 {( "G2" 8 )} 
wvSelectSignal -win $_nWave2 {( "G2" 8 )} 
wvSelectSignal -win $_nWave2 {( "G2" 8 )} 
wvSelectSignal -win $_nWave2 {( "G2" 8 )} 
wvSelectSignal -win $_nWave2 {( "G2" 8 )} 
wvSelectSignal -win $_nWave2 {( "G2" 8 )} 
wvSelectSignal -win $_nWave2 {( "G2" 8 )} 
wvSelectSignal -win $_nWave2 {( "G3" 14 )} 
wvSelectSignal -win $_nWave2 {( "G2" 6 )} 
wvSelectSignal -win $_nWave2 {( "G2" 8 )} 
verdiWindowResize -win $_Verdi_1 "65" "52" "1855" "1028"
verdiWindowResize -win $_Verdi_1 -532 "298" "1855" "1056"
wvSelectSignal -win $_nWave2 {( "G2" 6 7 8 )} 
wvSelectSignal -win $_nWave2 {( "G2" 8 )} 
wvSetPosition -win $_nWave2 {("G2" 8)}
wvExpandBus -win $_nWave2 {("G2" 8)}
wvSetPosition -win $_nWave2 {("G3" 14)}
wvSelectSignal -win $_nWave2 {( "G2" 8 )} 
wvSetPosition -win $_nWave2 {("G2" 8)}
wvCollapseBus -win $_nWave2 {("G2" 8)}
wvSetPosition -win $_nWave2 {("G2" 8)}
wvSetPosition -win $_nWave2 {("G3" 14)}
wvSetPosition -win $_nWave2 {("G2" 12)}
verdiWindowResize -win $_Verdi_1 -532 "326" "1855" "1028"
wvSelectSignal -win $_nWave2 {( "G2" 8 )} 
wvSelectSignal -win $_nWave2 {( "G2" 8 )} 
wvSetCursor -win $_nWave2 338084.454061 -snap {("G2" 8)}
wvSetCursor -win $_nWave2 343256.867415 -snap {("G3" 12)}
wvSetCursor -win $_nWave2 352308.590784 -snap {("G3" 12)}
wvSetCursor -win $_nWave2 333881.868210 -snap {("G2" 8)}
verdiWindowResize -win $_Verdi_1 "503" "95" "1855" "1056"
wvSelectSignal -win $_nWave2 {( "G2" 9 )} 
wvSelectSignal -win $_nWave2 {( "G2" 8 )} 
srcTBBTreeSelect -win $_nTrace1 -path "maxpooling"
srcTBBTreeSelect -win $_nTrace1 -path "maxpooling"
srcTBTreeAction -win $_nTrace1 -path "maxpooling"
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 0
wvScrollDown -win $_nWave2 9
wvScrollUp -win $_nWave2 9
srcSearchString "vallid" -win $_nTrace1 -next -case
srcSearchString "valid" -win $_nTrace1 -next -case
srcSearchString "valid" -win $_nTrace1 -next -case
srcDeselectAll -win $_nTrace1
srcSelect -signal "sram0_addr_dly" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "state_read_dly" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvSelectSignal -win $_nWave2 {( "G2" 9 )} 
wvSelectAll -win $_nWave2
wvCut -win $_nWave2
wvSetPosition -win $_nWave2 {("G2" 0)}
srcSelect -signal "clk" -win $_nTrace1
srcSelect -signal "rst_n" -win $_nTrace1
srcSelect -signal "ena" -win $_nTrace1
srcSelect -signal "frame_start_in" -win $_nTrace1
srcSelect -signal "line_start_in" -win $_nTrace1
srcSelect -signal "frame_end_in" -win $_nTrace1
srcSelect -signal "sig_layer" -win $_nTrace1
srcSelect -signal "max_layer" -win $_nTrace1
srcSelect -signal "frame_start_out" -win $_nTrace1
srcSelect -signal "line_start_out" -win $_nTrace1
srcSelect -signal "frame_end_out" -win $_nTrace1
srcSelect -signal "valid" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvZoom -win $_nWave2 7790404.040404 7998737.373737
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
debExit
