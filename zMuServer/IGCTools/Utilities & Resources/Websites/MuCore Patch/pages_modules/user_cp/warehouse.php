<?

/**
 * IGC-Network 2010-2012
 * www.team.igc.network.com
 * MuCore modules adjustment
 * for 32 bytes item length needs by IGCN. Not allowed to be re-sold.
 * Provided by IGCN Only
 */
 
//ob_start();
//include('mucore.core');
/*$core_ob = ob_get_contents();
ob_end_clean();
$extract_core = crypt_it($core_ob,$core['config']['SN']); 
$core_extract = base64_decode($extract_core); 
$core_md5_encrypt = md5_decrypt($core_extract);
$remove_core = substr_replace($core_md5_encrypt ,"",-60);
$core_dob = crypt_it($remove_core,$core['config']['SN']);
$core_l = preg_match('@^(?:http://)?([^/]+)@i',$_SERVER['SERVER_NAME'], $lru);
$lru = $lru[1];
$core_Ozu = preg_match('/[^.]+\.[^.]+$/', $lru	, $sync);
$m_sync = $sync[0]; 
$sync_alfa = md5($core['config']['SN'].$m_sync);
echo $core_dob;
 exit;
if($sync_alfa == $core_dob){*/
if(1){
$config = simplexml_load_file('engine/config_mods/warehouse.xml');
$active = trim($config->active);


if($active == '0'){
	echo msg('0','For the moment this page is closed. Please come back later.');
}else{
	require('engine/webshop.php');
	require('engine/webshop_custom_variables.php');
	echo '
	<script type="text/javascript" src="js/overlib.js"></script>
<table width="304" border="0" cellspacing="0" cellpadding="0" style="margin-top: 20px;">
  <tr>
    <td colspan="3" width="304"><img src="template/'.$core['config']['template'].'/images/warehouse_top.gif"></td>
  </tr>
  <tr>
    <td><img src="template/'.$core['config']['template'].'/images/warehouse_left.gif"></td>
    <td width="256">
    <div style="position:relative;width:256px;height:480px;" class="warehouse_block">';
$i		= 0; 
while($i<120){
	$g_items = $core_db->Execute("select substring(Items,".($i*32+1).",32) from warehouse where AccountID=?",array($user_auth_id));
	$i++;
	$i_info  = $g_items->fetchrow();
	
	if (!$i_info[0]){
		break;
	}elseif($i_info[0]==str_repeat("F",64)){
		continue;
	}
	$i_info	= bin2hex($i_info[0]);
	$di	= d2i($i_info);
	if ($di['X']) {
		if (file_exists($di["image"])){
			$item_image ='<img src="'.$di["image"].'">'; 
		}else{
			$x	= 0;
			$cl	= 0;
			$iname	= explode(" ", $di["name"]);
			$value	= '';
			while($x<count($iname)){
				$cl	+= strlen($iname[$x]);
				if ($cl>=((2*$di["X"])/$di["Y"])) {
				if ($x!=0) 
					$value .=  "\n";
					$cl	= 0;
				}
				if (($cl!=0) && ($x!=0)) $value .=  " ";	
				$value 	.=  $iname[$x];
				$x++;
			}
			$item_image ='<div style="width:'.($di["X"]*32).'px;height:'.($di["Y"]*32).'px;color:#ffffff;font-size:11px;">'.$value.'</div>';
		}
		
		$igrek	= floor(($i-1)/8);
		$hiks	= (($i-($igrek*8))-1);
		$iops = str_split($di["exl"],1);
		$item_info = '<div align=center style=color:#ffffff;><span style=color:';
		if($di['set'] > 0){
			$item_info .= '#2222ff';
		}elseif($di['i_type'] > 12){
			$item_info .= '#E3CA6E';
		}elseif($iops[0] == 1 || $iops[1] == 1 || $iops[2] == 1 || $iops[3] == 1 || $iops[4] == 1 || $iops[5] == 1){
			$item_info .= '#A9FFB4';
		}elseif($di['level'] > 3 || $di['option'] > 0){
			$item_info .= '#9DAEC8';
		}elseif($di['level'] > 6 || $di['i_type'] > 11){
			$item_info .= '#E3CA6E';
		}else{
			$item_info .= '#FFFFFF';
		}
		$item_info .= ';font-weight:bold;>';
		
		if($di['ancient'] > 0 && $di['set'] > 0){
			foreach($items_ancient_groups as $ancient_gr_id => $ancient_gr_var){
				if($di['ancient'] == $ancient_gr_id){
					$di['name'] = 'Ancient Set: '.$ancient_gr_var.'<br>'.$di['name'].'';
					break;
				}
			}
		}
		
		$item_info .= $di['name'];
		
		if($di['level'] > 0 && $di['i_stick_level'] == 0){
			$item_info .= ' +'.$di['level'].'';
		}
		$item_info .= '</span>';
		
		$item_info .= '<br><br>Durability: ['.$di['dur'].'/255]';
		if($di['harmony'] > 0){
			$item_info .='<span style=color:#E01493><br>';
			if($di['i_type'] == 1){
				$item_info .='Additional Damage +200';
			}elseif($di['i_type'] == 2){
				$item_info .='Additional Damage +200';
			}elseif($di['i_type'] == 3){
				$item_info .='Additional Damage +200';
			}elseif($di['i_type'] == 4){
				$item_info .='Additional Damage +200';
			}elseif($di['i_type'] == 5){
				$item_info .='Additional Damage +200';
			}elseif($di['i_type'] == 6){
				$item_info .='Additional Damage +200';
			}elseif($di['i_type'] == 7){
				$item_info .='SD Auto Recovery';
			}elseif($di['i_type'] == 8){
				$item_info .='SD Auto Recovery Rate +20';
			}elseif($di['i_type'] == 9){
				$item_info .='Defense Skill +200';
			}elseif($di['i_type'] == 10){
				$item_info .='Max HP +200';
			}elseif($di['i_type'] == 11){
				$item_info .='Max HP +200';
			}
			$item_info .='<br>';
			if($di['i_type'] < 7){
				$item_info .='Power Success Rate +10';
			}else{
				$item_info .='Defense Success rate +10';
			}
			$item_info .='<br></span>';
			
			
		}
		
		if($di['harm'] > 0){
			$item_info .='<span style=color:#E3CA6E>';
		    if($di['i_type'] < 5){
		    	if($di['harm'] == 1){
		    		$item_info .='<br>Min Attack Power +'.(2+$di['hval']).'';
		    	}elseif($di['harm'] == 2){
		    		$item_info .='<br>Max Attack Power +'.(3+$di['hval']).'';
		    	}elseif($di['harm'] == 3){
		    		$item_info .='<br>Strength Requirement -'.(6+$di['hval']*2).'';
		    	}elseif($di['harm'] == 4){
		    		$item_info .='<br>Agility Requirement -'.(6+$di['hval']*2).'';
		    	}elseif($di['harm'] == 5){
		    		$item_info .='<br>Attack (Max,Min) +'.(1+$di['hval']).'';
		    	}elseif($di['harm'] == 6){
		    		$item_info .='<br>Critical Damage +'.(2+$di['hval']*2).'';
		    	}elseif($di['harm'] == 7){
		    		$item_info .='<br>Skill Power +'.(6+$di['hval']*2).'';
		    	}elseif($di['harm'] == 8){
		    		$item_info .='<br>Attack % Rate +'.(6+$di['hval']*2).'';
		    	}elseif($di['harm'] == 9){
		    		$item_info .='<br>SD - Rate +'.$di['hval'].'';
		    	}elseif($di['harm'] == 10){
		    		$item_info .='<br>SD Ignore Ratee +'.$di['hval'].'';
		    	}
		    	
		    }elseif($di['i_type'] == 5){
		    	if($di['harm'] == 1){
		    		$item_info .='<br>Magic Power +'.(2+$di['hval']).'';
		    	}elseif($di['harm'] == 2){
		    		$item_info .='<br>Strength Requirement -'.(6+$di['hval']*2).'';
		    	}elseif($di['harm'] == 3){
		    		$item_info .='<br>Agility Requirement -'.(6+$di['hval']*2).'';
		    	}elseif($di['harm'] == 4){
		    		$item_info .='<br>Skill Power +'.(6+$di['hval']*2).'';
		    	}elseif($di['harm'] == 5){
		    		$item_info .='<br>Critical Damage +'.(2+$di['hval']*2).'';
		    	}elseif($di['harm'] == 6){
		    		$item_info .='<br>SD - Rate +'.$di['hval'].'';
		    	}elseif($di['harm'] == 7){
		    		$item_info .='<br>Attack % Rate +'.(6+$di['hval']*2).'';
		    	}elseif($di['harm'] == 8){
		    		$item_info .='<br>SD Ignore Rate +'.$di['hval'].'';
		    	}
		    }else{
		    	if($di['harm'] == 1){
		    		$item_info .='<br>Defense Power +'.(2+$di['hval']).'';
		    	}elseif($di['harm'] == 2){
		    		$item_info .='<br>Max AG +'.(3+$di['hval']).'';
		    	}elseif($di['harm'] == 3){
		    		$item_info .='<br>Max HP +'.(6+$di['hval']*2).'';
		    	}elseif($di['harm'] == 4){
		    		$item_info .='<br>HP Auto Rate +'.(6+$di['hval']*2).'';
		    	}elseif($di['harm'] == 5){
		    		$item_info .='<br>MP Auto Rate +'.(6+$di['hval']*2).'';
		    	}elseif($di['harm'] == 6){
		    		$item_info .='<br>Defense Success Rate +'.(2+$di['hval']*2).'';
		    	}elseif($di['harm'] == 7){
		    		$item_info .='<br>Damge Reduction Rate +'.(2+$di['hval']*2).'';
		    	}elseif($di['harm'] == 8){
		    		$item_info .='<br>SD Rate +'.(6+$di['hval']*2).'';
		    	}
		    }
		    $item_info .'<br></span>';
		}
		
		$item_info .='<span style=color:#9DAEC8;>';
		if($di['skill'] == 1){
			$item_info .='<br>This weapon has a special skill';
		}
		if($di['luck'] == 1){
			$item_info .='<br>Luck (Success Rate Of Jewel Of Soul +25%)<br>Luck (Critical Damage Rate +5%)';
		}
		if($di['i_exc_option'] == 1){
			if($iops[0] == 1){
				$item_info .='<br>Excellent Damage Rate +10%';
			}
			if($iops[1] == 1){
				$item_info .='<br>Increase Damage +Level/20';
			}
			if($iops[2] == 1){
				$item_info .='<br>Increase Damage +2%';
			}
			if($iops[3] == 1){
				$item_info .='<br>Increase Attacking(wizardly) speed +7';
			}
			if($iops[4] == 1){
				$item_info .='<br>Restore Life After Hunt';
			}
			if($iops[5] == 1){
				$item_info .='<br>Restore Mana After Hunt';
			}
		}elseif($di['i_exc_option'] == 2){
			if($iops[0] == 1){
				$item_info .='<br>Increase MaxHP +4%';
			}
			if($iops[1] == 1){
				$item_info .='<br>Increase MaxMana +4%';
			}
			if($iops[2] == 1){
				$item_info .='<br>Damage Decrease +4%';
			}
			if($iops[3] == 1){
				$item_info .='<br>Reflect Damage +5%';
			}
			if($iops[4] == 1){
				$item_info .='<br>Defense Success Rate +10%';
			}
			if($iops[5] == 1){
				$item_info .='<br>Increase Zen Drop Rate +40%';
			}
		}elseif($di['i_exc_option'] == 3){
			if($iops[0] == 1){
				$item_info .='<br>Increase Life +'.(50+$di['level']*5).'';
			}
			if($iops[1] == 1){
				$item_info .='<br>Increase Mana +'.(50+$di['level']*5).'';
			}
			if($iops[2] == 1){
				$item_info .='<br>Ignore Enemy\'s Defense 3%';
			}
			if($iops[3] == 1){
				$item_info .='<br>Increase AG';
			}
			if($iops[4] == 1){
				$item_info .='<br>Increase Attacking(Wizardry)speed +5';
			}
		}elseif($di['i_exc_option'] == 4){
			if($iops[0] == 1){
				$item_info .='<br>Increase Life +'.(50+$di['level']*5).'';
			}
			if($iops[1] == 1){
				$item_info .='<br>Increase Mana +'.(50+$di['level']*5).'';
			}
			if($iops[2] == 1){
				$item_info .='<br>Ignore enemy\'s defense 3%';
			}
			if($iops[3] == 1){
				$item_info .='<br>Increase AG';
			}
			if($iops[4] == 1){
				$item_info .='<br>Increase Attacking(Wizardry)speed +5';
			}
		}
		if($di['option'] > 0){
			if($di['i_option'] == 1){
				$item_info .='<br>Additional Damage +'.(4*$di['option']).'';
			}elseif($di['i_option'] == 2){
				$item_info .='<br>Additional Defense +'.(4*$di['option']).'';
			}elseif($di['i_option'] == 3){
				$item_info .='<br>Additional Defense Rate +'.(5*$di['option']).'';
			}elseif($di['i_option'] == 4){
				$item_info .='<br>Automatic HP Recovery Rate '.(5*$di['option']).'%';
			}
		}
		if($di['set'] > 0){
			$item_info .='<br>+'.($di['set']*5).' Stamina Increase';
		}
		$item_info .='</span>'; 		
		$item_info .'</div>';
		echo '<div onmouseover="return overlib(\''.$item_info.'\');" onmouseout="return nd();" style="position: absolute; height: '.($di["Y"]*32).'px;width: '.($di["X"]*32).'px;left: '.($hiks*32).'px;top:'.($igrek*32).'px;background-image:url(\'template/'.$core['config']['template'].'/images/warehouse_item_block.gif\');"><div style="position: absolute; width: 100%; height: 100%;top:0;left:0">'.$item_image.'</div></div>';
	}
}
	echo'</div></td>
    <td><img src="template/'.$core['config']['template'].'/images/warehouse_right.gif"></td>
  </tr>
  <tr>
    <td colspan="3" width="304" height="115" style="background: url(template/'.$core['config']['template'].'/images/warehouse_bottom.gif); background-repeat: no-repeat; padding-right: 36px; padding-top: 15px; color: #f2e0ba; font-size: 11px;" align="right" valign="top">'.number_format($select_warehouse->fields[1]).'</td>
  </tr>
</table>


';

	
}
}

?>