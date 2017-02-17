<?php

/**
 * IGC-Network 2010-2012
 * www.team.igc.network.com
 * MuCore modules adjustment
 * for 32 bytes item length needs by IGCN. Not allowed to be re-sold.
 * Provided by IGCN Only
 */
 
function item_image($theid, $type, $ExclAnci, $lvl = 0)
{
    switch ($ExclAnci) {
        case 1:
            $tnpl = '10';
            break;
        case 2:
            $tnpl = '01';
            break;
        default:
            $tnpl = '00';
    }
    $itype = $type * 16;
    if ($theid > 63) {
        $nxt = dechex($theid);
    } elseif ($theid > 31) {
        $nxt = "F9";
        $theid -= 32;
    } else {
        $nxt = "00";
    }
    
    if ($itype < 128) {
        $tipaj = "00";
    } else {
        $tipaj = 80;
        $itype -= 128;
    }
    
    $theid += $itype;
    $itype += $theid;
    $itype = sprintf("%02X", $itype, 00);
    
    if (file_exists('webshop_items/' . $tnpl . $itype . $tipaj . $nxt . '.gif')) {
        $output = 'webshop_items/' . $tnpl . $itype . $tipaj . $nxt . '.gif';
    } else {
        $output = 'webshop_items/00' . $itype . $tipaj . $nxt . '.gif';
    }
    
    $i = $lvl + 1;
    while ($i > 0) {
        $i--;
        if (file_exists('webshop_items/' . $tnpl . $itype . $tipaj . $nxt . sprintf("%02X", $i, 00) . '.gif')) {
            $output = 'webshop_items/' . $tnpl . $itype . $tipaj . $nxt . sprintf("%02X", $i, 00) . '.gif';
            $i      = 0;
        }
    }
    return $output;
}

function itemslot($whbin, $itemX, $itemY)
{
    global $core_db;
    if (substr($whbin, 0, 2) == '0x')
        $whbin = substr($whbin, 2);
    $items  = str_repeat('0', 120);
    $itemsm = str_repeat('1', 120);
    $i      = 0;
    while ($i < 120) {
        $_item = substr($whbin, (64 * $i), 64);
        $type  = hexdec(substr($_item, 18, 1));
        $query = $core_db->Execute("select top 1 size_x,size_y from MUCore_Shop_Items where i_id='" . hexdec(substr($_item, 0, 2)) . "' and i_type='" . $type . "'");
        $query = $query->fetchrow();
        $y     = 0;
        while ($y < $query[1]) {
            $y++;
            $x = 0;
            while ($x < $query[0]) {
                $items = substr_replace($items, '1', ($i + $x) + (($y - 1) * 8), 1);
                $x++;
            }
        }
        $i++;
    }
    $y = 0;
    while ($y < $itemY) {
        $y++;
        $x = 0;
        while ($x < $itemX) {
            $x++;
            $spacerq[$x + (8 * ($y - 1))] = true;
        }
    }
    $walked = 0;
    $i      = 0;
    while ($i < 120) {
        if (isset($spacerq[$i])) {
            $itemsm = substr_replace($itemsm, '0', $i - 1, 1);
            $last   = $i;
            $walked++;
        }
        if (@$walked == count($spacerq))
            $i = 119;
        $i++;
    }
    $useforlength     = substr($itemsm, 0, $last);
    $findslotlikethis = '^' . str_replace('++', '+', str_replace('1', '+[0-1]+', $useforlength));
    $i                = 0;
    $nx               = 0;
    $ny               = 0;
    while ($i < 120) {
        if ($nx == 8) {
            $ny++;
            $nx = 0;
        }
        if ((eregi($findslotlikethis, substr($items, $i, strlen($useforlength)))) && ($itemX + $nx < 9) && ($itemY + $ny < 16))
            return $i;
        $i++;
        $nx++;
    }
    return 1337;
}


function d2i($icode)
{
    global $core_db;
    if (substr($icode, 0, 2) == '0x') {
        $icode = substr($icode, 2);
    }
    if ((strlen($icode) != 64) || (!ereg("(^[a-zA-Z0-9])", $icode)) || ($icode == str_repeat("F", 64))) {
        return false;
    }
    list($refop, $luck, $skill, $iopx6, $iopx5, $iopx4, $iopx3, $iopx2, $iopx1, $ac, $item_harmony, $tipche) = array(
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0
    );
    $aa       = hexdec(substr($icode, 0, 2)); // Item ID
    $bb       = hexdec(substr($icode, 2, 2)); // Item Level/Skill/Option Data
    $cc       = hexdec(substr($icode, 4, 2)); // Item Durability
    $ddeeffgg = substr($icode, 6, 8); // Item SKey
    $hh       = hexdec(substr($icode, 14, 2)); // Item Excellent Info/ Option
    $ii       = hexdec(substr($icode, 16, 2)); // Ancient data
    $jj       = hexdec(substr($icode, 18, 1)); // Item Type
    $_ref     = hexdec(substr($icode, 19, 1)); // Refinery
    $k        = hexdec(substr($icode, 20, 1)); // Harmony type
    $l        = hexdec(substr($icode, 21, 1)); // Garmony Value
    $itemtype = $jj;
    if ($ii == 5)
        $ii = 1;
    elseif ($ii == 9)
        $ii = 2;
    if ($bb > 127) {
        $skill = 1;
        $bb -= 128;
    }
    $itemlevel = floor($bb / 8);
    $bb -= $itemlevel * 8;
    if ($bb > 3) {
        $luck = 1;
        $bb -= 4;
    }
    if ($hh >= 64) {
        $bb += 4;
        $hh -= 64;
    }
    if ($hh >= 32) {
        $iopx6 = 1;
        $hh -= 32;
        $tipche = 1;
    }
    if ($hh >= 16) {
        $iopx5 = 1;
        $hh -= 16;
        $tipche = 1;
    }
    if ($hh >= 8) {
        $iopx4 = 1;
        $hh -= 8;
        $tipche = 1;
    }
    if ($hh >= 4) {
        $iopx3 = 1;
        $hh -= 4;
        $tipche = 1;
    }
    if ($hh >= 2) {
        $iopx2 = 1;
        $hh -= 2;
        $tipche = 1;
    }
    if ($hh >= 1) {
        $iopx1 = 1;
        $hh -= 1;
        $tipche = 1;
    }
    if ($ii > 0)
        $tipche = 2;
    $fquery = $core_db->Execute("select ancient_id,i_type,i_id,name,i_stick_level,i_option,i_exc_option,i_socket_option,i_harmony_option,size_x,size_y from MUCore_Shop_Items where i_id=" . $aa . " and i_type=" . $itemtype . " and i_stick_level=" . $itemlevel);
    if ($fquery->EOF) {
        $fquery  = $core_db->Execute("select ancient_id,i_type,i_id,name,i_stick_level,i_option,i_exc_option,i_socket_option,i_harmony_option,size_x,size_y from MUCore_Shop_Items where i_id=" . $aa . " and i_type=" . $itemtype);
        $nolevel = 0;
    } else {
        $nolevel = 1;
    }
    if ($_ref == 8) {
        $nolevel      = 0;
        $item_harmony = 1;
    }
    if ($fquery->EOF) {
        return false;
    }
    $output['ancient']       = $fquery->fields[0];
    $output['id']            = $aa;
    $output['name']          = $fquery->fields[3];
    $output['i_stick_level'] = $fquery->fields[4];
    $output['i_type']        = $fquery->fields[1];
    $output['i_option']      = $fquery->fields[5];
    $output['i_exc_option']  = $fquery->fields[6];
    $output['level']         = $itemlevel;
    $output['socket']        = $fquery->fields[7];
    $output['canRefine']     = $fquery->fields[8];
    $output['option']        = $bb;
    $output['exl']           = $iopx6 . $iopx5 . $iopx4 . $iopx3 . $iopx2 . $iopx1;
    $output['luck']          = $luck;
    $output['skill']         = $skill;
    $output['harm']          = $k;
    $output['hval']          = $l;
    $output['harmony']       = $item_harmony;
    $output['dur']           = $cc;
    $output['X']             = $fquery->fields[9];
    $output['Y']             = $fquery->fields[10];
    $output['set']           = $ii;
    $output['image']         = item_image($fquery->fields[2], $fquery->fields[1], $tipche, $itemlevel);
    $output['sn']            = $ddeeffgg;
    return $output;
}


function webshop_log($user, $item_serial, $content)
{
    global $core_db;
    $insert_log_w = $core_db->Execute("INSERT INTO MUCore_Shop_Logs(memb___id,content,date_time,item_serial)VALUES('$user','$content'," . time() . ",'$item_serial')");
}

function dec_to_hex($dec) 
{ 
    $sign = ""; // suppress errors 
    
    if( $dec < 0){ $sign = "-"; $dec = abs($dec); } 

    $hex = Array( 0 => 0, 1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5, 
                  6 => 6, 7 => 7, 8 => 8, 9 => 9, 10 => 'a', 
                  11 => 'b', 12 => 'c', 13 => 'd', 14 => 'e',    
                  15 => 'f' ); 
    $counter = 0;    
    do 
    { 
//        echo floor(fmod($dec,16)).'|';
//        echo $dec.'|';
//        echo $hex[($dec%16)].'<br>';
//        if($counter>8) {
//       $H = $hex[($dec%16)] . $H;     
//        } else {
//        $h = $hex[($dec%16)] . $h; 
//        }
        $counter++;
         if($counter>8) {
       $H = $hex[floor(fmod($dec,16))] . $H;     
        } else {
        $h = $hex[floor(fmod($dec,16))] . $h; 
        }
        $dec /= 16; 
        
    } 
    while( $dec >= 1 ); 
    
    return array(hexdec($h),hexdec($H),$h); 
} 
?> 