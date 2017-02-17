<img src='depo.jpeg'/>

<?php 

/**
 * IGC-Network 2010-2012
 * www.team.igc.network.com
 * MuCore modules adjustment
 * for 32 bytes item length needs by IGCN. Not allowed to be re-sold.
 * Provided by IGCN Only
 */
 
ob_start();
include('mucore.core');
$core_ob = ob_get_contents();
ob_end_clean();

if($sync_alfa == $core_dob){
	

$webshop_settings = simplexml_load_file('engine/config_mods/webshop_settings.xml');
if($webshop_settings->active == '0'){
	echo msg('0',text_sorry_feature_disabled);
}else{
	require('engine/webshop_custom_variables.php');
	require('engine/webshop.php');
	$display_columns = $webshop_settings->columns;
	$display_classs = $webshop_settings->display_class;
	
	$item_level_limit = $webshop_settings->limit_level;
	$item_option_limit = $webshop_settings->limit_option;
	$item_excelent_limit = $webshop_settings->limit_exc_option;
	$item_harmony_limit = '10';
	
	
	$credits_level = $webshop_settings->credits_level;
	$credits_option = $webshop_settings->credits_option;
	$credits_skill = $webshop_settings->credits_skill;
	$credits_luck = $webshop_settings->credits_luck;
	$credits_ancient1 = $webshop_settings->credits_ancient1;
	$credits_ancient2 = $webshop_settings->credits_ancient2;
	$credits_refined = $webshop_settings->credits_refined;
	$credits_harmony = $webshop_settings->credits_harmony;
	
	$credits_w6 = $webshop_settings->credits_weapon1;
	$credits_w5 = $webshop_settings->credits_weapon2;
	$credits_w4 = $webshop_settings->credits_weapon3;
	$credits_w3 = $webshop_settings->credits_weapon4;
	$credits_w2 = $webshop_settings->credits_weapon5;
	$credits_w1 = $webshop_settings->credits_weapon6;
	
	$credits_ar6 = $webshop_settings->credits_armor1;
	$credits_ar5 = $webshop_settings->credits_armor2;
	$credits_ar4 = $webshop_settings->credits_armor3;
	$credits_ar3 = $webshop_settings->credits_armor4;
	$credits_ar2 = $webshop_settings->credits_armor5;
	$credits_ar1 = $webshop_settings->credits_armor6;
	
	$credits_wn1 = $webshop_settings->credits_wing1;
	$credits_wn2 = $webshop_settings->credits_wing2;
	$credits_wn3 = $webshop_settings->credits_wing3;
	$credits_wn4 = $webshop_settings->credits_wing4;
	$credits_wn5 = $webshop_settings->credits_wing5;
	$credits_wn6 = '0';
	
	
	if(isset($_GET['buy_item']) && is_numeric($_GET['buy_item'])){
		$item_id = safe_input($_GET['buy_item'],'');
		$item = $core_db->Execute("Select i_sell,i_stock,name,i_max_level,i_stick_level,i_skill_option,i_luck_option,i_exc_option,i_option,i_max_option,ancient_id,i_harmony_option,i_type,i_refined_option,credits,i_image,i_id,i_max_excelent_option,size_x,size_y,i_default_durability,id,exc_anc from MUCore_Shop_Items where id=?",array($item_id));
		if($item->EOF){
			echo msg('0','Unable to buy item, reason: system error, please contact administrator.');
		}else{
			if($item->fields[3] == 0 || empty($item->fields[3])){
				$item->fields[3] = $item_level_limit;
			}
			
		    if($item->fields[9] == 0 || empty($item->fields[9])){
				$item->fields[9] = $item_option_limit;
			}
			
			if($item->fields[17]== 0 || empty($item->fields[17])){
				$item->fields[17] = $item_excelent_limit;
			}
			
			if($item->fields[10] != 0 || !empty($item->fields[10])){
				$ancient_item = '1';
			}
			
			$set_image = trim($item->fields[15]);
			if(!empty($set_image)){
				$item_image = 'webshop_items/'.$item->fields[15].'';
			}else{
				$item_image = item_image($item->fields[16],$item->fields[12],'0',$item->fields[4]);
			}
			
			$item_new_serial = 1;

						
			if($item->fields[0] != '1'){
				echo msg('0','Unable to buy item, reason: this item is not for sell.');
			}else{
				if(is_numeric($item->fields[1]) && $item->fields[1] <= 0){
					echo msg('0','Unable to buy item, reason: this item is out of stock.');
				}else{
					if(isset($_POST['process_item'])){
				
						if(account_online($user_auth_id) === true){
							echo msg('0','Unable to buy this item, reason: your account is connected in game, please disconnect.');	
						}else{
					$xl		= 0;
					$hop	= 0;
					$rq_credits	= $item->fields[14];
					
					if (!$item->fields[4] && $item->fields[3] > 0){
						$ilvl = safe_input($_POST["level"],'');
					}else{
						$ilvl = 0;
					}
					if ($item->fields[9] > 0 && @$_POST['item_optionS'] > 0){
						$iopt = safe_input($_POST['item_optionS'],'');
					}else{
						$iopt = 0;
					}
					
					if ($ancient_item == 1){
						$ianc = safe_input($_POST['item_ancient'],'');
					}else{
						$ianc = 0;
					}
					
					if ($ilvl > $item->fields[3]){
						$ilvl = $item->fields[3];
					}
					
					if ($ilvl < 0){
						$ilvl = 0;
					}
					
					$rq_credits	+= $ilvl*$credits_level;
					
					if ($iopt > $item->fields[9]){
						$iopt = $item->fields[9];
					}
					
					if ($iopt<0){
						$iopt=0;
					}
					
					$rq_credits	+= $iopt*$credits_option;
					
		            $ii=0;
		            if (@$_POST["item_refined"] == 1 && @$item->fields[11] == 1) {
		            	$rq_credits+=$credits_refined;
		            	$addrefined = 1;
		            }else{
		            	$addrefined=false;
		            }
		            
		           
		            if ($_POST['item_harmony'] > 0 && $item->fields[11] == 1 && $_POST['item_harmony']<17) {
		            	$addharmony = safe_input($_POST["item_harmony"],'');
		            	$rq_credits	+= $credits_harmony;
		            }else{
		            	$addharmony=0;
		            }
		            
		            if ($ianc==1){
		            	$rq_credits	+= $credits_ancient1;
		            	$ii		= 5;
		            }elseif ($ianc==2){
		            	$rq_credits	+= $credits_ancient2;
		            	$ii		= 9;
		            }
		            
		            if ($item->fields[7] != 0) {
		            	$xlcount	= 0;
		            	if (@$_POST["item_option1"] == "1") {
		            		$xlcount++;
		            		$xl+=1;
		            		switch ($item->fields[7]) { 
		            			case 1: $rq_credits+=$credits_w1; break;
		            			case 2: $rq_credits+=$credits_ar1; break;
		            			case 3: $rq_credits+=$credits_wn1; break;
		            			case 4: $rq_credits+=$credits_wn1; break;
		            		}
		            	}
		            	
		            if (@$_POST["item_option2"] == "1") {
		            		$xlcount++;
		            		$xl+=2;
		            		switch ($item->fields[7]) { 
		            			case 1: $rq_credits+=$credits_w2; break;
		            			case 2: $rq_credits+=$credits_ar2; break;
		            			case 3: $rq_credits+=$credits_wn2; break;
		            			case 4: $rq_credits+=$credits_wn2; break;
		            		}
		            	}
		            	
		            if (@$_POST["item_option3"] == "1") {
		            		$xlcount++;
		            		$xl+=4;
		            		switch ($item->fields[7]) { 
		            			case 1: $rq_credits+=$credits_w3; break;
		            			case 2: $rq_credits+=$credits_ar3; break;
		            			case 3: $rq_credits+=$credits_wn3; break;
		            			case 4: $rq_credits+=$credits_wn3; break;
		            		}
		            	}
		            	
		            if (@$_POST["item_option4"] == "1") {
		            		$xlcount++;
		            		$xl+=8;
		            		switch ($item->fields[7]) { 
		            			case 1: $rq_credits+=$credits_w4; break;
		            			case 2: $rq_credits+=$credits_ar4; break;
		            			case 3: $rq_credits+=$credits_wn4; break;
		            			case 4: $rq_credits+=$credits_wn4; break;
		            		}
		            	}
		            	
		            if (@$_POST["item_option5"] == "1") {
		            		$xlcount++;
		            		$xl+=16;
		            		switch ($item->fields[7]) { 
		            			case 1: $rq_credits+=$credits_w5; break;
		            			case 2: $rq_credits+=$credits_ar5; break;
		            			case 3: $rq_credits+=$credits_wn5; break;
		            			case 4: $rq_credits+=$credits_wn5; break;
		            		}
		            	}
		            	
		            if (@$_POST["item_option6"] == "1") {
		            		$xl+=32;
		            		switch ($item->fields[7]) { 
		            			case 1: $rq_credits+=$credits_w6; $xlcount++; break;
		            			case 2: $rq_credits+=$credits_ar6; $xlcount++; break;
		            			case 3: $rq_credits+=$credits_wn6; break;
		            			case 4: $rq_credits+=$credits_wn6; break;
		            		}
		            	}


		            	if ($xlcount > $item->fields[17]) {
		            		$error_message = msg('0','Unable to buy this item, reason: you are allowed to select only '.$item->fields[17].' options.');
		            		$noadd	= 1;
		            	}
		            }
		            if ($_POST["item_luck"] == "1" && $item->fields[6] == 1) {
		            	$rq_credits+=$credits_luck;
		            	$hop+=4;
		            }
		            
		            if ($_POST["item_skill"] == "1" && $item->fields[5] ==1) {
		            	$rq_credits+=$credits_skill;
		            	$hop+=128;
		            }
		            if ($iopt>=4) {
		            	$hop+=$iopt-4;
		            	$xl	+=64;
		            }else{
		            	$hop+= $iopt;
		            }
		            
		            if ($item->fields[4]){
		            	$ilvl=$item->fields[4];
		            }
		            
		            $hop	+= $ilvl*8;
     
		            if ($item_new_serial == 1) {
                     		$serial_take =  $core_db->Execute("exec WZ_GetItemSerial2 2");
		            	$serial = $serial_take->fetchRow();
		            	echo $serial	= $serial[0];
		            }else{
		            	$serial='00000000';
		            }
                     // echo '<br>';
                          $hex = dec_to_hex($serial);
                          //  print_r($hex);
		            $dur	= $item->fields[20];
		            $itemhex	= sprintf("%02X",$item->fields[16],00);
		            $itemhex	.= sprintf("%02X",$hop,00);
		            $itemhex	.= sprintf("%02X",$dur,00);
		            $itemhex	.= sprintf("%08X",$hex[0],00000000);
		            $itemhex	.= sprintf("%02X",$xl,00);	
		            $itemhex	.= sprintf("%02X",$ii,00);
		            $itemhex	.= dechex($item->fields[12]);
                            
		            if (@$addrefined){
		            	$itemhex.='8';
		            }else{ 
		            	$itemhex.='0';
		            }
		            
		            $itemhex	.= dechex($addharmony);
		            $itemhex	.= dechex($item_harmony_limit);
		            $itemhex	.= "0000000000";
                            $itemhex	.= sprintf("%08X",$hex[1],00000000);
                            $itemhex	.= 'FFFFFFFFFFFFFFFFFFFFFFFF'; //not realized atm
                           // echo $itemhex;//exit;
                           // echo $itemhex; exit;
		            $test		= 0;
		            $i			= 0;
		            $mycuritems	= '0x';
		             
		            while ($i<120) {
		            	$query = $core_db->Execute("select substring(Items,".($i*64+1).",64) from warehouse where AccountID=?",array($user_auth_id));
		            	$i++;
		            	$query = $query->fetchrow();
		            	$mycuritems	.= bin2hex($query[0]);
		            }
		            
		         
		           
		            
		            $slot 		= itemslot($mycuritems,$item->fields[18],$item->fields[19]);
                           // echo '<br>slot:'.$slot;
                           // echo '<br>$mycuritems:'.$mycuritems;
		            $test		= $slot*64;
		            
					if ($slot==1337)  {
						if(!$error_message){
							$error_message = msg('0','Unable to buy this item, reason: you don\'t have enoguh space in your warehouse.');
						}
						$noadd=1;
					}
					
					$take_credits = $core_db2->Execute("Select credits from MEMB_CREDITS where memb___id=?",array($user_auth_id));
					if($take_credits->fields[0] < $rq_credits){
						if(!$error_message){
							$error_message = msg('0','Unable to buy this item, reason: you don\'t have enough credits.');
						}
						$noadd=1;
					}
					
					if ($noadd != 1) {
						$update_credits = $core_db2->Execute("Update MEMB_CREDITS set credits=(credits-?) where memb___id=?",array($rq_credits,$user_auth_id));
						$insert_item = $core_db->Execute("update warehouse set Items=".substr_replace($mycuritems, $itemhex, ($test+2), 64)." where AccountID=?",array($user_auth_id));
						if($item->fields[1] > 0){
							$update_stock = $core_db->Execute("Update MUCore_Shop_Items set i_stock=(i_stock-1) where id=?",array($item->fields[21]));
						}
						webshop_log($user_auth_id,$itemhex,'Purchased '.$item->fields[2].' for '.number_format($rq_credits).' credits.');
						
						$error_message = msg('1','Item have been successfully purchased.');
						
					}
					
					
					
					echo $error_message;
					}
					
					}else{
						echo '
						<script type="text/javascript">

						function credits_amount(){
							amount = '.$item->fields[14].';
							
						';
					  
					    if($item->fields[3] > 0 && $item->fields[4] == 0){
					    	echo '
					    	update_amount=document.getElementById(\'credits_level\').innerHTML;
							if (update_amount>0){
								amount+=parseInt(update_amount);
								}
							 ';
					    }
					    
					    if($item->fields[5] == 1){
					    	echo '
					    	update_amount=document.getElementById(\'credits_skill\').innerHTML;
							if (update_amount>0){
								amount+=parseInt(update_amount);
								}
					    	';
					    }
					    
					    if($item->fields[6] == 1){
					    	echo '
					    	update_amount=document.getElementById(\'credits_luck\').innerHTML;
							if (update_amount>0){
								amount+=parseInt(update_amount);
								}
					    	';
					    }
					    
					    if($item->fields[8] != 0 && $item->fields[9] > 0){
					    	echo '
					    	
					    	update_amount=document.getElementById(\'credits_optionS\').innerHTML;
							if (update_amount>0){
								amount+=parseInt(update_amount);
								}';
					    }
					    
					    
					    if($item->fields[11] == 1){
					    	echo '
					    	update_amount=document.getElementById(\'credits_harmony\').innerHTML;
							if (update_amount>0){
								amount+=parseInt(update_amount);
								}
								';
					    	
					    }
					    
					    if($item->fields[13] == 1){
					    	echo '
					    	
					    	update_amount=document.getElementById(\'credits_refined\').innerHTML;
							if (update_amount>0){
								amount+=parseInt(update_amount);
								}
								

								';
					    	
					    }
					    
					    if($item->fields[10] != 0){
					    	echo '
					    	
					    	update_amount=document.getElementById(\'credits_ancient\').innerHTML;
							if (update_amount>0){
								amount+=parseInt(update_amount);
								}
								';
					    	
					    }
					   if($item->fields[7] != 0){
					    	echo '
					    
					    	update_amount=document.getElementById(\'total_credits_options\').value;
							if (update_amount>0){
								amount+=parseInt(update_amount);
								}
								';
					    	
					    }
					    

					    
					    echo '
					    
					    document.getElementById(\'total_credits\').innerHTML=amount;
						}
						';
					    
					   echo '
					   function check_options(){ 
					      count = 0;
					   '; 
					   if($item->fields[7] == 1){
					   	echo '
					   	value_option1='.$credits_w1.';
					   	value_option2='.$credits_w2.';
					   	value_option3='.$credits_w3.';
					   	value_option4='.$credits_w4.';
					   	value_option5='.$credits_w5.';
					   	value_option6='.$credits_w6.';
					   	';
					   	
					   }elseif ($item->fields[7] == 2){
					   	echo '
					   	value_option1='.$credits_ar1.';
					   	value_option2='.$credits_ar2.';
					   	value_option3='.$credits_ar3.';
					   	value_option4='.$credits_ar4.';
					   	value_option5='.$credits_ar5.';
					   	value_option6='.$credits_ar6.';
					   	';
					   }elseif($item->fields[7] == 3){
					   	echo '
					   	value_option1='.$credits_wn1.';
					   	value_option2='.$credits_wn2.';
					   	value_option3='.$credits_wn3.';
					   	value_option4='.$credits_wn4.';
					   	value_option5='.$credits_wn5.';
					   	value_option6='.$credits_wn6.';
					   	';
					   }elseif($item->fields[7] == 4){
					   	echo '
					   	value_option1='.$credits_wn1.';
					   	value_option2='.$credits_wn2.';
					   	value_option3='.$credits_wn3.';
					   	value_option4='.$credits_wn4.';
					   	value_option5='.$credits_wn5.';
					   	value_option6='.$credits_wn6.';
					   	';
					   }
					   
					   echo '
					   value_credits=0;
					  
					   if (document.getElementById(\'item_option1\').checked == 1) { 
					        count++;
					        value_credits+=value_option1; 
					        document.getElementById(\'credits_option1\').innerHTML=value_option1;
					        document.getElementById(\'credits_option1_css\').className=\'shop_credits_enabled\';
					   }else{
					        document.getElementById(\'credits_option1\').innerHTML=0;
					        document.getElementById(\'credits_option1_css\').className=\'shop_credits_disabled\';
					   
					   }
					   
					  
					   
					   if (document.getElementById(\'item_option2\').checked == 1) {
					       count++;
					       value_credits+=value_option2; 
					       document.getElementById(\'credits_option2\').innerHTML=value_option2;
					       document.getElementById(\'credits_option2_css\').className=\'shop_credits_enabled\';
					   }else{
					        document.getElementById(\'credits_option2\').innerHTML=0;
					        document.getElementById(\'credits_option2_css\').className=\'shop_credits_disabled\';
					   
					   }
					   
					   if (document.getElementById(\'item_option3\').checked == 1) {
					       count++;
					       value_credits+=value_option3; 
					       document.getElementById(\'credits_option3\').innerHTML=value_option3;
					       document.getElementById(\'credits_option3_css\').className=\'shop_credits_enabled\';
					   }else{
					        document.getElementById(\'credits_option3\').innerHTML=0;
					        document.getElementById(\'credits_option3_css\').className=\'shop_credits_disabled\';
					   
					   }
					    
					   
					   if (document.getElementById(\'item_option4\').checked == 1) {
					       count++;
					       value_credits+=value_option4; 
					       document.getElementById(\'credits_option4\').innerHTML=value_option4;
					       document.getElementById(\'credits_option4_css\').className=\'shop_credits_enabled\';
					   }else{
					        document.getElementById(\'credits_option4\').innerHTML=0;
					        document.getElementById(\'credits_option4_css\').className=\'shop_credits_disabled\';
					   
					   }
					   
					   
					   if (document.getElementById(\'item_option5\').checked == 1) {
					       count++;
					       value_credits+=value_option5; 
					       document.getElementById(\'credits_option5\').innerHTML=value_option5;
					       document.getElementById(\'credits_option5_css\').className=\'shop_credits_enabled\';
					   }else{
					        document.getElementById(\'credits_option5\').innerHTML=0;
					        document.getElementById(\'credits_option5_css\').className=\'shop_credits_disabled\';
					   
					   }';
					   
					   if($item->fields[7] < 3){
					   	echo'if (document.getElementById(\'item_option6\').checked == 1) {
					       count++;
					       value_credits+=value_option6; 
					       document.getElementById(\'credits_option6\').innerHTML=value_option6;
					       document.getElementById(\'credits_option6_css\').className=\'shop_credits_enabled\';
					   }else{
					        document.getElementById(\'credits_option6\').innerHTML=0;
					        document.getElementById(\'credits_option6_css\').className=\'shop_credits_disabled\';
					   
					   }';
					   }
					   echo'
					   
					  
					   if (count>'.$item->fields[17].'){
					      alert(\'You are allowed to select only '.$item->fields[17].' options.\');
					      
					      document.getElementById(\'item_option1\').checked = false;
					      document.getElementById(\'credits_option1\').innerHTML=0;
					      document.getElementById(\'credits_option1_css\').className=\'shop_credits_disabled\';
					      value_credits-=value_option1; 
					      
					      document.getElementById(\'item_option2\').checked = false;
					      document.getElementById(\'credits_option2\').innerHTML=0;
					      document.getElementById(\'credits_option2_css\').className=\'shop_credits_disabled\';
					      value_credits-=value_option2; 
					      
					      document.getElementById(\'item_option3\').checked = false;
					      document.getElementById(\'credits_option3\').innerHTML=0;
					      document.getElementById(\'credits_option3_css\').className=\'shop_credits_disabled\';
					      value_credits-=value_option3; 
					      
					      document.getElementById(\'item_option4\').checked = false;
					      document.getElementById(\'credits_option4\').innerHTML=0;
					      document.getElementById(\'credits_option4_css\').className=\'shop_credits_disabled\';
					      value_credits-=value_option4; 
					      
					      document.getElementById(\'item_option5\').checked = false;
					      document.getElementById(\'credits_option5\').innerHTML=0;
					      document.getElementById(\'credits_option5_css\').className=\'shop_credits_disabled\';
					      value_credits-=value_option5; 
					      
					      ';
					   
					   if($item->fields[7] < 3){
					   	echo
					   	'document.getElementById(\'item_option6\').checked = false;
					      document.getElementById(\'credits_option6\').innerHTML=0;
					      document.getElementById(\'credits_option6_css\').className=\'shop_credits_disabled\';
					      ';
					   }
					   echo'
					   
					   }
					     document.getElementById(\'total_credits_options\').value=value_credits;
					   ';
					   if($item->fields[22] != 1){
					   
					   if($item->fields[10] != 0){
					   	echo 'if(count > 0){
					      document.getElementById(\'item_ancient\').value=0;
					      document.getElementById(\'credits_ancient_css\').className=\'shop_credits_disabled\';
					      document.getElementById(\'credits_ancient\').innerHTML=\'0\';
					      document.getElementById(\'ancient_div\').style.visibility = \'hidden\'; 
					   }';
					   }
					   	
					   	
					   }
					   
					   echo'
					   credits_amount(); 
					   }
					   
					   ';
					    
					    
					    
					    echo '
					    function item_level() { 
							 value = document.getElementById(\'level\').value; 
							 if(value > 0){
							    document.getElementById(\'credits_level_css\').className=\'shop_credits_enabled\';
							 }else{
							    document.getElementById(\'credits_level_css\').className=\'shop_credits_disabled\';
							 }
							 
							 document.getElementById(\'credits_level\').innerHTML=(document.getElementById(\'level\').value*'.$credits_level.'); 
							 credits_amount(); 
							}
							
							
					    function check_skill() { 
					    	if (document.getElementById(\'item_skill\').checked == 1){
					    		document.getElementById(\'credits_skill\').innerHTML='.$credits_skill.';
					    		document.getElementById(\'credits_skill_css\').className=\'shop_credits_enabled\';
					    	}else{
					    		document.getElementById(\'credits_skill\').innerHTML=0;
					    		document.getElementById(\'credits_skill_css\').className=\'shop_credits_disabled\';
					    	}
					    	
					    	credits_amount(); 
					    }
					    
					    function check_luck() { 
					    	if (document.getElementById(\'item_luck\').checked == 1){
					    		document.getElementById(\'credits_luck\').innerHTML='.$credits_luck.';
					    		document.getElementById(\'credits_luck_css\').className=\'shop_credits_enabled\';
					    	}else{
					    		document.getElementById(\'credits_luck\').innerHTML=0;
					    		document.getElementById(\'credits_luck_css\').className=\'shop_credits_disabled\';
					    	}
					    	credits_amount(); 
					    }
					    
					    function check_optionS(){ 
					       value = document.getElementById(\'item_optionS\').value;
						   
						   if (value > 0){
						       document.getElementById(\'credits_optionS_css\').className=\'shop_credits_enabled\';
						   }else {
						       document.getElementById(\'credits_optionS_css\').className=\'shop_credits_disabled\'; 
						   } 
						   
						   document.getElementById(\'credits_optionS\').innerHTML=(document.getElementById(\'item_optionS\').value*'.$credits_option.');   
						   credits_amount(); 
						}
						
						function check_harmony(){ 
					       value = document.getElementById(\'item_harmony\').value;
						   if (value > 0){
						       document.getElementById(\'credits_harmony\').innerHTML='.$credits_harmony.'; 
						       document.getElementById(\'credits_harmony_css\').className=\'shop_credits_enabled\';
						   }else {
						       document.getElementById(\'credits_harmony\').innerHTML=0; 
						       document.getElementById(\'credits_harmony_css\').className=\'shop_credits_disabled\'; 
						   }
						   credits_amount(); 
						}
						
						function check_refined() { 
					    	if (document.getElementById(\'item_refined\').checked == 1){
					    		document.getElementById(\'credits_refined\').innerHTML='.$credits_refined.';
					    		document.getElementById(\'credits_refined_css\').className=\'shop_credits_enabled\';
					    	}else{
					    		document.getElementById(\'credits_refined\').innerHTML=0;
					    		document.getElementById(\'credits_refined_css\').className=\'shop_credits_disabled\';
					    	}
					    	credits_amount(); 
					    }
					    
					    
					    function check_ancient() {
						   if (document.getElementById(\'item_ancient\').value> 0){
						   ';
					    if($item->fields[22] != 1){
					    if($item->fields[7] != 4){
					    echo '
						       document.getElementById(\'credits_option1_css\').className=\'shop_credits_disabled\';
						       document.getElementById(\'item_option1\').checked=false;
						       document.getElementById(\'credits_option1\').innerHTML=0;
						       
						       document.getElementById(\'credits_option2_css\').className=\'shop_credits_disabled\';
						       document.getElementById(\'item_option2\').checked=false;
						       document.getElementById(\'credits_option2\').innerHTML=0;
						       
						       
						       document.getElementById(\'credits_option3_css\').className=\'shop_credits_disabled\';
						       document.getElementById(\'item_option3\').checked=false;
						       document.getElementById(\'credits_option3\').innerHTML=0;
						       
						       document.getElementById(\'credits_option4_css\').className=\'shop_credits_disabled\';
						       document.getElementById(\'item_option4\').checked=false;
						       document.getElementById(\'credits_option4\').innerHTML=0;
						       
						       document.getElementById(\'credits_option5_css\').className=\'shop_credits_disabled\';
						       document.getElementById(\'item_option5\').checked=false;
						       document.getElementById(\'credits_option5\').innerHTML=0;
						       ';
					    if($item->fields[7] != '3'){
					    	echo'
					    
						       document.getElementById(\'credits_option6_css\').className=\'shop_credits_disabled\';
						       document.getElementById(\'item_option6\').checked=false;
						       document.getElementById(\'credits_option6\').innerHTML=0;';
					        }
					      }
					    }
					    
					    echo'
					    

						      
						       document.getElementById(\'credits_ancient_css\').className=\'shop_credits_enabled\';
						       
						       document.getElementById(\'ancient_div\').style.visibility = \'visible\'; 
						       
						       if (document.getElementById(\'item_ancient\').value==1){
						           ancient_cost = '.$credits_ancient1.';
						       }else{
						           ancient_cost = '.$credits_ancient2.';
						       }
						       document.getElementById(\'credits_ancient\').innerHTML=ancient_cost;
						       ';
					    if($item->fields[7] != 4){
					    	echo 'check_options();';
					    }
					    echo'
						      
						   } else {
						     document.getElementById(\'credits_ancient_css\').className=\'shop_credits_disabled\';
						   }
						   if (document.getElementById(\'item_ancient\').value<1) {
						       document.getElementById(\'credits_ancient_css\').className=\'shop_credits_disabled\';
						       document.getElementById(\'credits_ancient\').innerHTML=0;
						       
						       document.getElementById(\'ancient_div\').style.visibility = \'hidden\'; 
						   } 
						   credits_amount(); 
						   
						   }

						   </script>
	
					
				<div style="margin-top: 10px; margin-bottom: 10px;">
				<fieldset><legend>Buy '.htmlentities($item->fields[2]).'</legend>';
			    if($item->fields[10] != 0){
			    	foreach($items_ancient_groups as $ac_gr_id => $ac_gr_var){
			    		if($ac_gr_id == $item->fields[10]){
			    			echo '<div style="margin-left: 17px; margin-top: 15px; visibility:hidden;" id="ancient_div"><span class="shop_ancient">Ancient Set: '.$ac_gr_var.'</span></div>';
			    			break;
			    		}
			    	}
			    	
			    }
			    echo '
			    <form action="" method="POST" name="buy_item">
				<table width="100%" border="0" align="center" cellpadding="0" cellspacing="4" style="padding-left: 10px; padding-right: 10px;">
				<tr>
				<td align="left" valign="top" width="70%">
				<table width="100%" border="0" align="center" cellpadding="0" cellspacing="4">
				';
					
						if($item->fields[3] > 0 && $item->fields[4] == 0){
							echo 
							'<tr>
				<td align="left" width="200"><b>Item level:</b></td>
				<td align="left"><select name="level" id="level" onKeyUp="javascript:item_level();" onChange="javascript:item_level();">
							<option value="0">+0</option>';
							$i=0;
							while($i < $item->fields[3]){
								$i++;
								echo '<option value="'.$i.'">+'.$i.'</option>';
							}
							echo'</select> <span id="credits_level_css" class="shop_credits_disabled"><span id="credits_level">0</span> Credits</span></td></tr>';
						}else{
							if(!empty($item->fields[4])){
								echo '
							<tr>
				<td align="left" width="200"><b>Item level:</b></td>
				<td align="left">+'.$item->fields[4].'</td></tr>';
							}
							
						}
						
						
						
						if($item->fields[5] == '1'){
							echo '<tr>
							<td align="left">Skill Option:</td>
							<td align="lett"><label><input type="checkbox" name="item_skill" id="item_skill" onChange="javascript:check_skill();" value="1"> Yes</lable> <span id="credits_skill_css" class="shop_credits_disabled"><span id="credits_skill">0</span> Credits</span></td>
							</tr>';
						}
						
						
						if($item->fields[6] == '1'){
							echo '<tr>
							<td align="left">Luck Option:</td>
							<td align="lett"><label><input type="checkbox" name="item_luck" id="item_luck" onChange="javascript:check_luck();" value="1"> Yes</lable> <span id="credits_luck_css" class="shop_credits_disabled"><span id="credits_luck">0</span> Credits</span></td>
							</tr>';
						}	
						
						
						//op1
						if($item->fields[7] == '1'){
							echo '<tr>
							<td align="left">Excellent Damage Rate +10%:</td>
							<td align="lett"><label><input type="checkbox" name="item_option6" id="item_option6" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option6_css" class="shop_credits_disabled"><span id="credits_option6">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '2'){
							echo '<tr>
							<td align="left">Increase MaxHP +4%:</td>
							<td align="lett"><label><input type="checkbox"  name="item_option6" id="item_option6" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option6_css" class="shop_credits_disabled"><span id="credits_option6">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '3'){
							echo '<tr>
							<td align="left">Increase Life:</td>
							<td align="lett"><label><input type="checkbox"  name="item_option1" id="item_option1" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option1_css" class="shop_credits_disabled"><span id="credits_option1">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '4'){
							echo '<tr>
							<td align="left">Ignore Enemy\'s Defense 5%:</td>
							<td align="lett"><label><input type="checkbox"  name="item_option1" id="item_option1" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option1_css" class="shop_credits_disabled"><span id="credits_option1">0</span> Credits</span></td>
							</tr>';
						}						
						
						//op2
						if($item->fields[7] == '1'){
							echo '<tr>
							<td align="left">Increase Damage +Level/20:</td>
							<td align="lett"><label><input type="checkbox" name="item_option5" id="item_option5" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option5_css" class="shop_credits_disabled"><span id="credits_option5">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '2'){
							echo '<tr>
							<td align="left">Increase MaxMP +4%:</td>
							<td align="lett"><label><input type="checkbox" name="item_option5" id="item_option5" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option5_css" class="shop_credits_disabled"><span id="credits_option5">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '3'){
							echo '<tr>
							<td align="left">Increase Mana:</td>
							<td align="lett"><label><input type="checkbox" name="item_option2" id="item_option2" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option2_css" class="shop_credits_disabled"><span id="credits_option2">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '4'){
							echo '<tr>
							<td align="left">5% Reflect Probabilities:</td>
							<td align="lett"><label><input type="checkbox" name="item_option2" id="item_option2" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option2_css" class="shop_credits_disabled"><span id="credits_option2">0</span> Credits</span></td>
							</tr>';
						}	

						//op3
						if($item->fields[7] == '1'){
							echo '<tr>
							<td align="left">Increase Damage +2%:</td>
							<td align="lett"><label><input type="checkbox" name="item_option4" id="item_option4" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option4_css" class="shop_credits_disabled"><span id="credits_option4">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '2'){
							echo '<tr>
							<td align="left">Damage Decrease +4%:</td>
							<td align="lett"><label><input type="checkbox" name="item_option4" id="item_option4" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option4_css" class="shop_credits_disabled"><span id="credits_option4">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '3'){
							echo '<tr>
							<td align="left">Ignore Enemy\'s Defense 3%:</td>
							<td align="lett"><label><input type="checkbox" name="item_option3" id="item_option3" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option3_css" class="shop_credits_disabled"><span id="credits_option3">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '4'){
							echo '<tr>
							<td align="left">5% Probabilities Complete Life Recovery:</td>
							<td align="lett"><label><input type="checkbox" name="item_option3" id="item_option3" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option3_css" class="shop_credits_disabled"><span id="credits_option3">0</span> Credits</span></td>
							</tr>';
						}
						
						//op4
						if($item->fields[7] == '1'){
							echo '<tr>
							<td align="left">Increase Wizardly Speed +7:</td>
							<td align="lett"><label><input type="checkbox" name="item_option3" id="item_option3" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option3_css" class="shop_credits_disabled"><span id="credits_option3">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '2'){
							echo '<tr>
							<td align="left">Reflect Damage +5%:</td>
							<td align="lett"><label><input type="checkbox" name="item_option3" id="item_option3" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option3_css" class="shop_credits_disabled"><span id="credits_option3">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '3'){
							echo '<tr>
							<td align="left">Increase AG:</td>
							<td align="lett"><label><input type="checkbox" name="item_option4" id="item_option4" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option4_css" class="shop_credits_disabled"><span id="credits_option4">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '4'){
							echo '<tr>
							<td align="left">Rolls Up With But 5% Complete Recover:</td>
							<td align="lett"><label><input type="checkbox" name="item_option4" id="item_option4" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option4_css" class="shop_credits_disabled"><span id="credits_option4">0</span> Credits</span></td>
							</tr>';
						}
											
					   //op5
						if($item->fields[7] == '1'){
							echo '<tr>
							<td align="left">Restore Life After Hsunt:</td>
							<td align="lett"><label><input type="checkbox" name="item_option2" id="item_option2" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option2_css" class="shop_credits_disabled"><span id="credits_option2">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '2'){
							echo '<tr>
							<td align="left">Defense Success Rate +10%:</td>
							<td align="lett"><label><input type="checkbox" name="item_option2" id="item_option2" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option2_css" class="shop_credits_disabled"><span id="credits_option2">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '3'){
							echo '<tr>
							<td align="left">Increase Wizardly Speed +5:</td>
							<td align="lett"><label><input type="checkbox" name="item_option5" id="item_option5" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option5_css" class="shop_credits_disabled"><span id="credits_option5">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '4'){
							echo '<input type="hidden" name="item_option5" id="item_option5" value="0"><span id="credits_option5_css" class="shop_credits_disabled"><span id="credits_option5">
							';
						}
						
						//op6
						if($item->fields[7] == '1'){
							echo '<tr>
							<td align="left">Restore Mana After Hunt:</td>
							<td align="lett"><label><input type="checkbox" name="item_option1" id="item_option1" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option1_css" class="shop_credits_disabled"><span id="credits_option1">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '2'){
							echo '<tr>
							<td align="left">Increase Zen Drop Rate +40%:</td>
							<td align="lett"><label><input type="checkbox" name="item_option1" id="item_option1" onChange="javascript:check_options();" value="1"> Yes</lable> <span id="credits_option1_css" class="shop_credits_disabled"><span id="credits_option1">0</span> Credits</span></td>
							</tr>';
						}elseif($item->fields[7] == '3'){
							echo '<input type="hidden" name="item_option6" id="item_option6" value="1"';
						}elseif($item->fields[7] == '4'){
							echo '<input type="hidden" name="item_option6" id="item_option6" value="1"';
						}
						
						if($item->fields[8] != 0 && $item->fields[9] > 0){
							$select_java_optionS = 'onKeyUp="javascript:check_optionS();" onChange="javascript:check_optionS();"';
							$java_optionS = '<span id="credits_optionS_css" class="shop_credits_disabled"><span id="credits_optionS">0</span> Credits</span>';
						
							if($item->fields[8]  == '1'){
								echo '<tr>
							<td align="left"><b>Additional Damage:</b></td>
							<td align="left"><select name="item_optionS" id="item_optionS" '.$select_java_optionS.'>
							<option value="0">+0</option>';
								$a=0;
							    while($a < $item->fields[9]){
									$a++;
									echo '<option value="'.$a.'">+'.($a*4).'</option>';
								}
								echo '</select> '.$java_optionS .'</td></tr>';
								
							}elseif($item->fields[8]  == '2'){
								echo '<tr>
							    <td align="left"><b>Additional Defense:</b></td>
							    <td align="left"><select name="item_optionS" id="item_optionS" '.$select_java_optionS.'>
							    <option value="0">+0</option>';
								$a=0;
								while($a < $item->fields[9]){
									$a++;
									echo '<option value="'.$a.'">+'.($a*4).'</option>';
								}
								echo '</select> '.$java_optionS .'</td></tr>';
							}elseif($item->fields[8]  == '3'){
								echo '<tr>
							    <td align="left"><b>Addional Defense Rate:</b></td>
							    <td align="left"><select name="item_optionS" id="item_optionS" '.$select_java_optionS.'>
							    <option value="0">+0</option>';
								$a=0;
								while($a < $item->fields[9]){
									$a++;
									echo '<option value="'.$a.'">+'.($a*5).'</option>';
								}
								echo '</select> '.$java_optionS .'</td></tr>';
							}elseif($item->fields[8]  == '4'){
								echo '<tr>
							    <td align="left"><b>Automatic HP Recovery:</b></td>
							    <td align="left"><select name="item_optionS" id="item_optionS" '.$select_java_optionS.'>
							    <option value="0">0%</option>';
								$a=0;
								while($a < $item->fields[9]){
									$a++;
									echo '<option value="'.$a.'">'.($a*1).'%</option>';
								}
								echo '</select> '.$java_optionS .'</td></tr>';
							}
						}
						
						if($ancient_item == '1'){
							echo '<tr>
							<td align="left"><b>Stamina Increase:</b></td>
							<td align="left">
							<select name="item_ancient" id="item_ancient" onKeyUp="javascript:check_ancient();" onChange="javascript:check_ancient();">
							<option value="0">+0</option>
							<option value="1">+5</option>
							<option value="2">+10</option>
							</select> <span id="credits_ancient_css" class="shop_credits_disabled"><span id="credits_ancient">0</span> Credits</span></td></tr>';
						}
						
						if($item->fields[11] == '1'){
							echo '<tr>
							<td align="left"><b>Harmony:</b></td>
							<td align="left"><select name="item_harmony" id="item_harmony" onKeyUp="javascript:check_harmony();" onChange="javascript:check_harmony();">
							<option value="0">No Harmony</option>';
							if($item->fields[12] < 5){
								echo '
								<option value="1">Min Attack Power </option>
                                <option value="2">Max Attack Power</option>
                                <option value="3">Strength Requirement</option>
                                <option value="4">Agility Requirement</option>
                                <option value="5">Attack (Max,Min) </option>
                                <option value="6">Critical Damage</option>
                                <option value="7">Skill Power </option>
                                <option value="8">Attack % Rate </option>
                                <option value="9">SD - Rate</option>
                                <option value="10">SD Ignore Rate </option>';
							}elseif($item->fields[12] == 5){
								echo '
								<option value="1">Magic Power</option>
                                <option value="2">Strength Requirement</option>
                                <option value="3">Agility Requirement</option>
                                <option value="4">Skill Power</option>
                                <option value="5">Critical Damage</option>
                                <option value="6">SD - Rate </option>
                                <option value="7">Attack % Rate </option>
                                <option value="8">SD Ignore Rate </option>';
							}else{
								echo '
								<option value="1">Defense Power</option>
                                <option value="2">Max AG</option>
                                <option value="3">Max HP</option>
                                <option value="4">HP Auto Rate</option>
                                <option value="5">MP Auro Rate</option>
                                <option value="6">Defense Success Rate</option>
                                <option value="7">Damge Reduction Rate</option>
                                <option value="8">SD Rate</option>';
							}
							
							echo '</select> <span id="credits_harmony_css" class="shop_credits_disabled"><span id="credits_harmony">0</span> Credits</span></td></tr>';
							
						}
						
						if($item->fields[13] == '1'){
							$item_refined_java_select = 'onChange="javascript:check_refined();" value="1"';
							$item_refined_java = '<span id="credits_refined_css" class="shop_credits_disabled"><span id="credits_refined">0</span> Credits</span>';
							if($item->fields[12] == '1'){
								echo '<tr>
							<td align="left"><b>Additional Damage +200:<br>Power Success Rate +10</b></td>
							<td align="lett"><label><input type="checkbox" name="item_refined" id="item_refined" '.$item_refined_java_select.'> Yes</lable> '.$item_refined_java.'</td>
							</tr>';
							}elseif($item->fields[12] == '2'){
								echo '<tr>
							<td align="left"><b>Additional Damage +200<br>Power Success Rate +10:</b></td>
							<td align="lett"><label><input type="checkbox" name="item_refined" id="item_refined" '.$item_refined_java_select.'> Yes</lable> '.$item_refined_java.'</td>
							</tr>';
							}elseif($item->fields[12] == '3'){
								echo '<tr>
							<td align="left"><b>Additional Damage +200<br>Power Success Rate +10:</b></td>
							<td align="lett"><label><input type="checkbox" name="item_refined" id="item_refined" '.$item_refined_java_select.'> Yes</lable> '.$item_refined_java.'</td>
							</tr>';
							}elseif($item->fields[12] == '4'){
								echo '<tr>
							<td align="left"><b>Additional Damage +200<br>Power Success Rate +10:</b></td>
							<td align="lett"><label><input type="checkbox" name="item_refined" id="item_refined" '.$item_refined_java_select.'> Yes</lable> '.$item_refined_java.'</td>
							</tr>';
							}elseif($item->fields[12] == '5'){
								echo '<tr>
							<td align="left"><b>Additional Damage +200<br>Power Success Rate +10:</b></td>
							<td align="lett"><label><input type="checkbox" name="item_refined" id="item_refined" '.$item_refined_java_select.'> Yes</lable> '.$item_refined_java.'</td>
							</tr>';
							}elseif($item->fields[12] == '6'){
								echo '<tr>
							<td align="left"><b>Additional Damage +200<br>Power Success Rate +10:</b></td>
							<td align="lett"><label><input type="checkbox" name="item_refined" id="item_refined" '.$item_refined_java_select.'> Yes</lable> '.$item_refined_java.'</td>
							</tr>';
							}elseif($item->fields[12] == '7'){
								echo '<tr>
							<td align="left"><b>SD Auto Recovery<br>Defense Success rate +10:</b></td>
							<td align="lett"><label><input type="checkbox" name="item_refined" id="item_refined" '.$item_refined_java_select.'> Yes</lable> '.$item_refined_java.'</td>
							</tr>';
							}elseif($item->fields[12] == '8'){
								echo '<tr>
							<td align="left"><b>SD Auto Recovery Rate +20<br>Defense Success rate +10:</b></td>
							<td align="lett"><label><input type="checkbox" name="item_refined" id="item_refined" '.$item_refined_java_select.'> Yes</lable> '.$item_refined_java.'</td>
							</tr>';
							}elseif($item->fields[12] == '9'){
								echo '<tr>
							<td align="left"><b>Defense Skill +200<br>Defense Success rate +10:</b></td>
							<td align="lett"><label><input type="checkbox" name="item_refined" id="item_refined" '.$item_refined_java_select.'> Yes</lable> '.$item_refined_java.'</td>
							</tr>';
							}elseif($item->fields[12] == '10'){
								echo '<tr>
							<td align="left"><b>Max HP +200<br>Defense Success rate +10:</b></td>
							<td align="lett"><label><input type="checkbox" name="item_refined" id="item_refined" '.$item_refined_java_select.'> Yes</lable> '.$item_refined_java.'</td>
							</tr>';
							}elseif($item->fields[12] == '11'){
								echo '<tr>
							<td align="left"><b>Max HP +200<br>Defense Success rate +10:</b></td>
							<td align="lett"><label><input type="checkbox" name="item_refined" id="item_refined" '.$item_refined_java_select.'> Yes</lable> '.$item_refined_java.'</td>
							</tr>';
							}else{
								echo '<tr>
							<td align="left"><b>Defense Success rate +10:</b></td>
							<td align="lett"><label><input type="checkbox" name="item_refined" id="item_refined" '.$item_refined_java_select.'> Yes</lable> '.$item_refined_java.'</td>
							</tr>';
							}
					
								
							
						}
						
						
						echo '
				</table>
				</td>
				<td align="center" valign="top" width="30%"><div align="center" class="shop_image"><img src="'.$item_image.'"></div></td>
				</tr>
				</table>
				<table width="100%" border="0" align="center" cellpadding="0" cellspacing="4" style="padding-left: 10px; padding-right: 10px;">
				<tr>
				<td style="background-image:url(template/'.$core['config']['template'].'/images/inner_line.jpg); background-repeat:repeat-x; height: 2px;" colspan="2">
				</td>
				</tr>
				</table> 
				<table width="100%" border="0" align="center" cellpadding="0" cellspacing="4" style="padding-left: 10px; padding-right: 10px; padding-bottom: 5px;">        
				<tr>
				<td align="left"><b>Total Credits Cost</b> : <span id="total_credits">'.$item->fields[14].'</span> <input type="hidden" id="total_credits_options" name="total_credits_options" value="0"></td>
				<td align="right">
				<input type="hidden" name="process_item">
			<input type="submit" value="Buy this item" onclick="return ask_form(\'Are you sure you want to buy this item for \'+document.getElementById(\'total_credits\').innerHTML+\' Credits?\')">

			</td>
                </tr>
  
				</table></form>
				</fildset>
				</div>';
					}
					
				}		
			}
		}
		
	}else{
		if(isset($_GET['item_category']) && is_numeric($_GET['item_category'])){
			$item_category_gid = safe_input($_GET['item_category'],'');
			if(array_key_exists($item_category_gid,$items_categories)){
				$category_pressent = '1';
			}
		}
		if(isset($_GET['class']) && is_numeric($_GET['class']) && $category_pressent=='1'){
			$class_gid = safe_input($_GET['class'],'');
			if(array_key_exists($class_gid,$characters_class)){
				$class_pressent = '1';
			}
		}
			echo '<div class="iR_rank_type" style="margin-top: 3px;">
			<span style="color: #990000;">Items Category</a></div>
			<div class="iR_rank_type_sub shop_nav">';
			
			foreach ($items_categories as $item_category_id => $item_category_var){
				if($category_pressent == '1'){
					if($item_category_id == $item_category_gid){
						echo '<span class="shop_nav_seelcted">['.$item_category_var.']</span>&nbsp;&nbsp;';
					}else{
						echo '<a href="'.$core_run_script.'&item_category='.$item_category_id.'">'.$item_category_var.'</a>&nbsp;&nbsp;';
					}
				}else{
					echo '<a href="'.$core_run_script.'&item_category='.$item_category_id.'">'.$item_category_var.'</a>&nbsp;&nbsp;';
				}
			}
			echo '</div>';
		
		
		if($category_pressent == '1'){
			echo '<table width="100%" border="0" align="center" cellpadding="4" cellspacing="0">';
			
			   if($display_classs == 1){
				echo '
				<tr>
				<td align="left" colspan="'.$display_columns.'"><b>Class Filter:</b> 
				';
				if($class_pressent == '1'){
					echo '<a href="'.$core_run_script.'&item_category='.$item_category_gid.'">All</a>';
				}else{
					echo '[All]';
				}
				
				foreach($characters_class as $class_id => $class_var){
					if($class_pressent == '1'){
						if($class_gid == $class_id){
							echo ' - ['.$class_var[0].']';
						}else{
							echo ' - <a href="'.$core_run_script.'&item_category='.$item_category_gid.'&class='.$class_id.'">'.$class_var[0].'</a>';
						}
					}else{
							echo ' - <a href="'.$core_run_script.'&item_category='.$item_category_gid.'&class='.$class_id.'">'.$class_var[0].'</a>';
					}
				
				}
				echo'</td></tr>';
			   }
			   echo'
				<tr>
				<td colspan="'.$display_columns.'" style="background-image:url(template/'.$core['config']['template'].'/images/inner_line.jpg); background-repeat:repeat-x; height: 2px;"></td>
				</tr>';
			
			
			$columns_count = 0;
			$calc_width = 100/$display_columns;
			$select_items = $core_db->Execute("Select id,name,credits,i_image,i_id,i_type,i_stick_level,class_requirement,i_stock,ancient_id from MUCore_Shop_Items where category=? and i_sell='1' order by credits asc",array($item_category_gid));
			
			while(!$select_items->EOF){
				$columns_count++;
				$set_image = trim($select_items->fields[3]);
				if(!empty($set_image)){
					$item_image = 'webshop_items/'.$select_items->fields[3].'';
				}else{
					$item_image = item_image($select_items->fields[4],$select_items->fields[5],'0',$select_items->fields[6]);
				}
				if($class_pressent == '1'){
					$i_class_requirement = explode(',',$select_items->fields[7]);
					if(in_array($class_gid,$i_class_requirement)){
						echo '<td valign="top" align="center" width="'.round($calc_width).'%">
						<div align="center" class="shop_body_item">
						<div align="center" class="shop_title_item">'.htmlentities($select_items->fields[1]).'</div>
						';
					if($select_items->fields[9] > 0){
						foreach($items_ancient_groups as $ac_id => $ac_var){
							if($ac_id == $select_items->fields[9]){
								echo '<div align="center" style="margin-top: 10px;"><span class="shop_ancient">Ancient Set: '.$ac_var.'</span></div>';
								break;
							}
						}
					}
					
					
					echo '
						<div align="center" style="margin-top: 10px;"><img src="'.$item_image.'"></div>
						<div align="center" class="shop_credits_item">Credits: '.number_format($select_items->fields[2]).'</div>
						';
					if(is_numeric($select_items->fields[8])){
						if($select_items->fields[8] <= 0){
							echo '<div style="padding: 8px;"><span align="center" class="shop_stock_o">Out of stock!</span></div>';
						}else{
							echo '<div style="padding: 8px;"><span align="center" class="shop_stock_a">Items in stock: '.number_format($select_items->fields[8]).'</span></div>';
						}
						
					}
					
						if($display_classs == '1'){
							echo '<div align="center" class="shop_class_requirement"><b>Can be equiped by:</b><br>';
							$i_class_requirement = explode(',',$select_items->fields[7]);
							foreach($characters_class as $class_id => $class_var){
								if(in_array($class_id,$i_class_requirement)){
									echo $class_var[0].', ';
								}
							}
							echo '</div>';
						}
						echo'
						<div align="center" class="shop_button"><input type="button" value="Buy Item" onclick="location.href=\''.$core_run_script.'&buy_item='.$select_items->fields[0].'\'"></div>';
					if($_SESSION['admin_login_auth']){
							echo '<div align="center" style="margin-top:10px;margin-bottom:10px;"><a href="admincp/index.php?get=webshop_item_manager&m=edit&id='.$select_items->fields[0].'" target="_blank">[-Edit Item-]</a></div>';
						}
						echo'
						</div>
						';
					}
				}else{
					echo '<td valign="top" align="center" width="'.round($calc_width).'%">
				
					<div align="center" class="shop_body_item">
					<div align="center" class="shop_title_item">'.htmlentities($select_items->fields[1]).'</div>
					';
					if($select_items->fields[9] > 0){
						foreach($items_ancient_groups as $ac_id => $ac_var){
							if($ac_id == $select_items->fields[9]){
								echo '<div align="center" style="margin-top: 10px;"><span class="shop_ancient">Ancient Set: '.$ac_var.'</span></div>';
								break;
							}
						}
					}
					
					
					echo '
					
					<div align="center" style="margin-top: 10px;"><img src="'.$item_image.'"></div>
					
					<div align="center" class="shop_credits_item">Credits: '.number_format($select_items->fields[2]).'</div>
					';
					if(is_numeric($select_items->fields[8])){
						if($select_items->fields[8] <= 0){
							echo '<div style="padding: 8px;"><span align="center" class="shop_stock_o">Out of stock!</span></div>';
						}else{
							echo '<div style="padding: 8px;"><span align="center" class="shop_stock_a">Items in stock: '.number_format($select_items->fields[8]).'</span></div>';
						}
						
					}
					
					if($display_classs == '1'){
						echo '<div align="center" class="shop_class_requirement"><b>Can be equiped by:</b><br>';
						$i_class_requirement = explode(',',$select_items->fields[7]);
						foreach($characters_class as $class_id => $class_var){
							if(in_array($class_id,$i_class_requirement)){
								echo $class_var[0].', ';
							}
						}
						echo '</div>';
					}
					echo'
					
					<div align="center" class="shop_button">
					<input type="button" value="Buy Item" onclick="location.href=\''.$core_run_script.'&buy_item='.$select_items->fields[0].'\'"></div>';
						if($_SESSION['admin_login_auth']){
							echo '<div align="center" style="margin-top:10px;margin-bottom:10px;"><a href="admincp/index.php?get=webshop_item_manager&m=edit&id='.$select_items->fields[0].'" target="_blank">[-Edit Item-]</a></div>';
						}
					echo'
					</div>
					';
				}
				
				
				
				if ($columns_count%$display_columns){
					echo '</td>';
				}else{
					echo "</td></tr><tr>";
				}
				$select_items->MoveNext();
				
			}
			echo '</table>';
		}
		
	}
	
	
}
}
?>