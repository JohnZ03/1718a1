debImport "-f" "filelist.f"
debLoadSimResult \
           /home/wugxfly/Documents/cnn_mnist_1718/1718a1/forward_pass_top/conv_top_tb.fsdb
wvCreateWindow
verdiWindowResize -win $_Verdi_1 "490" "142" "900" "728"
verdiWindowResize -win $_Verdi_1 "55" -14 "1855" "1056"
srcHBSelect "conv_top_tb.dut" -win $_nTrace1
srcSetScope -win $_nTrace1 "conv_top_tb.dut" -delim "."
srcDeselectAll -win $_nTrace1
srcSelect -signal "clk" -win $_nTrace1
srcSelect -signal "rst_n" -win $_nTrace1
srcDeselectAll -win $_nTrace1
srcSelect -signal "ima" -win $_nTrace1
srcSelect -signal "rst_n" -win $_nTrace1
srcSelect -signal "clk" -win $_nTrace1
srcSelect -signal "frame_start_in" -win $_nTrace1
srcSelect -signal "frame_end_in" -win $_nTrace1
srcSelect -signal "line_start_in" -win $_nTrace1
srcSelect -signal "ena_in" -win $_nTrace1
srcSelect -signal "frame_start_dim_in" -win $_nTrace1
srcSelect -signal "frame_end_dim_in" -win $_nTrace1
srcSelect -toggle -signal "frame_end_dim_in" -win $_nTrace1
srcSelect -signal "frame_end_dim_in" -win $_nTrace1
srcSelect -signal "valid" -win $_nTrace1
srcSelect -signal "forward_pass_out" -win $_nTrace1
srcAddSelectedToWave -win $_nTrace1
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 56569711.538462 -snap {("G1" 11)}
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoom -win $_nWave2 55709134.615385 58653846.153846
wvZoom -win $_nWave2 57692921.655413 57770786.623978
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
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
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoom -win $_nWave2 54086538.461538 61778846.153846
wvZoom -win $_nWave2 57475036.982240 57830991.124252
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvSetCursor -win $_nWave2 56075178.866058 -snap {("G1" 5)}
wvSetMarker -win $_nWave2 57745000.000000
wvZoomIn -win $_nWave2
wvSetWindowTimeUnit -win $_nWave2 10.000000 ps
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomIn -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
wvZoomOut -win $_nWave2
debExit
