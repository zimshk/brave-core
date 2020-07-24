/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_CLASSIFICATION_PURCHASE_INTENT_CLASSIFIER_PURCHASE_INTENT_CLASSIFIER_USER_MODELS_H_  // NOLINT
#define BAT_ADS_INTERNAL_CLASSIFICATION_PURCHASE_INTENT_CLASSIFIER_PURCHASE_INTENT_CLASSIFIER_USER_MODELS_H_  // NOLINT

#include <map>
#include <set>
#include <string>

namespace ads {

const std::map<std::string, std::string> kPurchaseIntentCountryCodes = {
  {
    "AF", "jememeholcpjpoahinnlafoiaknnmfgl"
  },
  {
    "AX", "hfonhokmgmjionconfpknjfphfahdklo"
  },
  {
    "AL", "anlkmbkbgleadcacchhgdoecllpllknb"
  },
  {
    "DZ", "imoolhehjnpebcjecoinphmohihmbccj"
  },
  {
    "AS", "kgnhcdjacgcanjnbdcmngdeoncckfmfh"
  },
  {
    "AD", "pmlmnjficamnkblapnohndlnhkkoaoco"
  },
  {
    "AO", "majdffglhcddbbanjnhfocagmmhobghd"
  },
  {
    "AI", "bhdlolcjjefaidodgffjhpbeeapabpgn"
  },
  {
    "AQ", "pbanoihfljabneihobeplbciopfilajn"
  },
  {
    "AG", "cbdjliajiakicmdohhbjbgggacbjdnmn"
  },
  {
    "AR", "enadnicbppimlbpilkeaogmmfpennphn"
  },
  {
    "AM", "cpnhinhihfnhhmoknpbkcifgadjbcjnf"
  },
  {
    "AW", "ocegkjjbmlnibhfjmeiaidplhcbdhomd"
  },
  {
    "AU", "kklfafolbojbonkjgifmmkdmaaimminj"
  },
  {
    "AT", "jmneklmcodckmpipiekkfaokobhkflep"
  },
  {
    "AZ", "llmikniomoddmmghodjfbncbidjlhhid"
  },
  {
    "BS", "aaoipmkakdldlippoaocoegnnfnpcokj"
  },
  {
    "BH", "megoieebjempmobckocciojgfhfmiejb"
  },
  {
    "BD", "ppkgobeickbpfmmkbhfgmgiloepdiagn"
  },
  {
    "BB", "ndfnmlonkimafoabeafnaignianecdhf"
  },
  {
    "BY", "apndmjdcfbhgfccccdmmeofpdgnlbbif"
  },
  {
    "BE", "lnbdfmpjjckjhnmahgdojnfnmdmpebfn"
  },
  {
    "BZ", "demegfhfekncneocdlflandgegpcoofj"
  },
  {
    "BJ", "jiodmgmkikfbkchgenlamoabbfnobnfh"
  },
  {
    "BM", "aeiffmlccgeaacefkfknodmnebanekbo"
  },
  {
    "BT", "hemccombjdaepjnhhdplhiaedaackooa"
  },
  {
    "BO", "dggplmjbjalpdgndkigojpikblaiflke"
  },
  {
    "BQ", "jbibpedjodeigdoimlgpikphaabhdbie"
  },
  {
    "BA", "iefeaaegnnpiadjfoeifapjmflnbjehg"
  },
  {
    "BW", "bfdagcnfmfaidkkjcmfjmogiefoiglem"
  },
  {
    "BV", "kfimhlhdlhimaheficomcahhbaicoele"
  },
  {
    "BR", "fbpmbjccnaaeogogeldlomcmlhllgaje"
  },
  {
    "IO", "cpbmgmnfoakodmmpppabghhckchppneg"
  },
  {
    "BN", "gcikmigghagkligpileoekfjmohokjhm"
  },
  {
    "BG", "ahhbponhjohgifhjbjggafboffbimmmg"
  },
  {
    "BF", "fjgjapaemndekhnfeopeoeajfpmlgemo"
  },
  {
    "BI", "cfbbahiimladdkhblpkkokkmemlmkbhe"
  },
  {
    "CV", "oeneodeckioghmhokkmcbijfanjbanop"
  },
  {
    "KH", "cmknopomfihgdpjlnjhjkmogaooeoeic"
  },
  {
    "CM", "mmiflfidlgoehkhhkeodfdjpbkkjadgi"
  },
  {
    "CA", "gpaihfendegmjoffnpngjjhbipbioknd"
  },
  {
    "KY", "efpgpbmpbkhadlnpdnjigldeebgacopp"
  },
  {
    "CF", "ljfeoddejgcdofgnpgljaeiaemfimgej"
  },
  {
    "TD", "oahnhemdhgogkhgljdkhbficecbplmdf"
  },
  {
    "CL", "gbbfjnnpelockljikcmjkeodlaebjokm"
  },
  {
    "CN", "gfccfpdijajblfnkmddbflphiiibifik"
  },
  {
    "CX", "mjennfbimaafgcoekloojmbhnkophgni"
  },
  {
    "CC", "pnfogoijegjkepejdabehdfadbkpgoed"
  },
  {
    "CO", "cegiaceckhbagmmfcoeebeghiddghbkk"
  },
  {
    "KM", "efcmpmpbmaehimomnmonodlpnghelnfi"
  },
  {
    "CG", "hkgnnbjmfcelmehmphhbjigedknjobaa"
  },
  {
    "CD", "kignebofcmcgmjfiapilgdfkbekmkdmg"
  },
  {
    "CK", "clcghlkineckendfhkgdpkplofknofjo"
  },
  {
    "CR", "hmmoibmjgckbeejmcfflnngeacaklchb"
  },
  {
    "CI", "nopcbglolocphcdeikfkoeppkhijacij"
  },
  {
    "HR", "mjhnpmgafkmildljebajibghemlbffni"
  },
  {
    "CU", "mdopdmalncfakkimlojioichjbebcaip"
  },
  {
    "CW", "boeecnnjahpahhockgdcbdlaimpcflig"
  },
  {
    "CY", "hknminnkgcjafipipbbalkakppehkpjn"
  },
  {
    "CZ", "iejekkikpddbbockoldagmfcdbffomfc"
  },
  {
    "DK", "kmfkbonhconlbieplamnikedgfbggail"
  },
  {
    "DJ", "phihhhnelfclomhodhgalooldjgejgfl"
  },
  {
    "DM", "obihnhimgbeenjgfnlcdbfdgkkgeogdp"
  },
  {
    "DO", "gciaobanmdlfkegiikhgdoogegeghhch"
  },
  {
    "EC", "imnpbmpnmlmkjpgfnfeplikingjklpej"
  },
  {
    "EG", "ojfkdbfibflfejobeodhoepghgoghkii"
  },
  {
    "SV", "adnhangbagjlobdeicimblgcnafegpfb"
  },
  {
    "GQ", "gncihmgakhljdlkadibldhdfnocfplci"
  },
  {
    "ER", "diacfpapelanfbkmehdpaaohmnkkngge"
  },
  {
    "EE", "jdigggiplmjlocdopbdmjibckpamigij"
  },
  {
    "SZ", "npacefioaofgbofilfnhliofkefklipp"
  },
  {
    "ET", "lbiagcddmfapjfbebccolcahfppaimmo"
  },
  {
    "FK", "aogeahmaehgnkobkmnmkhkimdjpgfgen"
  },
  {
    "FO", "molpidmcmbmhbicckmbopbmiojddebke"
  },
  {
    "FJ", "biobhkionbllnfljaapocfpdmhamedkf"
  },
  {
    "FI", "ecneelfoifpgnignhipdebhbkgcphmic"
  },
  {
    "FR", "bgifagoclclhhoflocdefiklgodpihog"
  },
  {
    "GF", "mhmfclafeemiphfebhnobinkplbmpocm"
  },
  {
    "PF", "mjaianjdadeiocokpoanbgjhegficcce"
  },
  {
    "TF", "jbjodokafbafhemocebljdblgnfajabi"
  },
  {
    "GA", "nchncleokkkkdfgbgmenkpkmhnlbibmg"
  },
  {
    "GM", "alebifccfdpcgpandngmalclpbjpaiak"
  },
  {
    "GE", "kaikhlldfkdjgddjdkangjobahokefeb"
  },
  {
    "DE", "dgkplhfdbkdogfblcghcfcgfalanhomi"
  },
  {
    "GH", "panlkcipneniikplpjnoddnhonjljbdp"
  },
  {
    "GI", "pibapallcmncjajdoamgdnmgcbefekgn"
  },
  {
    "GR", "ochemplgmlglilaflfjnmngpfmpmjgnb"
  },
  {
    "GL", "gjknekmnjalchmfjbecifkoohcmlolhp"
  },
  {
    "GD", "kbllnlfaaoonbliendlhmoleflgdekki"
  },
  {
    "GP", "keggdlgkcfmmopgnogknokiocjjieapm"
  },
  {
    "GU", "mfnbeofelbcabhloibhpklffiifjcdfl"
  },
  {
    "GT", "jdhabeecpolnjdiplbcpjlohmlgdjgjh"
  },
  {
    "GG", "ncldbgolkmlgfkoignkdjehiadnfmlid"
  },
  {
    "GN", "gcfgdkmegcjaceeofigbabmjjiojdgnb"
  },
  {
    "GW", "kheejcjggceghjdinbcklehfkobloboc"
  },
  {
    "GY", "fehpbhdbjpnaibhncpeedfkogiiajcne"
  },
  {
    "HT", "pkpmecljhbjbiicbnfcfgpgmpehldemm"
  },
  {
    "HM", "kfjeoeekjccibpockdjcdjbgagaaopdj"
  },
  {
    "VA", "ljhceaiogeejahjjblnfaaocgogchpkb"
  },
  {
    "HN", "llmmfofcojgcignfnaodhdhdhphhmfmf"
  },
  {
    "HK", "plpcpclbpkilccbegfbpediidmejaahc"
  },
  {
    "HU", "pofhnfhkonpjephlcjlmbjmlikiaddoc"
  },
  {
    "IS", "cljplhmgochlncaelcabfgeefebhmfnk"
  },
  {
    "IN", "kdkakecfnmlfifkhlekmfkmmkpgeckcl"
  },
  {
    "ID", "lanimmipljlbdnajkabaoiklnpcaoakp"
  },
  {
    "IR", "mhiehehcmljjmpibmpiadoeefnchmbdm"
  },
  {
    "IQ", "oejhcomgcgcofdfchkdhkjohnioijofn"
  },
  {
    "IE", "fbmfelhaipnlicodibhjkmeafbcgpfnm"
  },
  {
    "IM", "blofecpleppfodonanffdbepbiijmklm"
  },
  {
    "IL", "fiodhmddlgkgajbhohfdmkliikidmaom"
  },
  {
    "IT", "gjkhegliajlngffafldbadcnpfegmkmb"
  },
  {
    "JM", "ncfbonfnhophngmkkihoieoclepddfhm"
  },
  {
    "JP", "ienmdlgalnmefnpjggommgdilkklopof"
  },
  {
    "JE", "lfgnglkpngeffaijiomlppnobpchhcgf"
  },
  {
    "JO", "gnkmfghehkoegoabhndflbdmfnlgeind"
  },
  {
    "KZ", "jadlfgggcfdhicaoacokfpmccbmedkim"
  },
  {
    "KE", "bfhpiebgnciehokapeppcinalnibbnan"
  },
  {
    "KI", "dkghhhflbpfknidjbhlophapheggpahk"
  },
  {
    "KP", "pnokpaenadbgpjpmlnoamnmpjbjlfoaf"
  },
  {
    "KR", "clgbjhhcdihjgbomhpmfdjdiagejadja"
  },
  {
    "KW", "ehkeinmpkojiiacjohbalbnhloiaifig"
  },
  {
    "KG", "hehalbiboicjbbcfhckdfeijjjppdhij"
  },
  {
    "LA", "lhjcndbhldpnapjddfgohdcdmfibgpon"
  },
  {
    "LV", "pooflbdadogbmjmnnppfmklfjbmoblfa"
  },
  {
    "LB", "hkengofpokonjepdafjdeclejledncdj"
  },
  {
    "LS", "mdojkinfephdfhbfadcnnfcjfniefael"
  },
  {
    "LR", "alenpijagefjamgompebcjhbfhepnphh"
  },
  {
    "LY", "mnhglgpnnohpipdeinibpbnlnpledicf"
  },
  {
    "LI", "onhaidkdpiboaolhnaddeddfaabomchd"
  },
  {
    "LT", "aokfbnlokidoepkhilbmfdkdhajkpbli"
  },
  {
    "LU", "gnmaofjfninpeccldcmlkbinhhohmbck"
  },
  {
    "MO", "ncmdondkaofghlnhiabnfilafhmabong"
  },
  {
    "MG", "lapgbedoccnchodbgfmafpkkhlfmcehe"
  },
  {
    "MW", "dhmcaoadkmoejegjpjgkjhibioemkfni"
  },
  {
    "MY", "dadpenhnclbbkjfbkgkgecknfjggpbmm"
  },
  {
    "MV", "ggclalmmmmgjcoleeficgnnjkpgeinfd"
  },
  {
    "ML", "flocoipmnbpcodjfhmkjecjpbkcmkecp"
  },
  {
    "MT", "emckddclmcjoilbadmdjdakabpnkdkhk"
  },
  {
    "MH", "cpjafhooepmhnflmjabfeaiopfbljhpo"
  },
  {
    "MQ", "chbeaiccoofemohdajloflfkblbgdiih"
  },
  {
    "MR", "dfmnoondmnbngeilibiicikjenjjeigi"
  },
  {
    "MU", "iobofpagkcicpcijjfmnghgppbghnpdo"
  },
  {
    "YT", "lcnaekpkllhpljanlibochejknjflodn"
  },
  {
    "MX", "dclpeadnefbjogjcamdglgmmbbgnjcob"
  },
  {
    "FM", "pjiglaefpchnekpbkbfngjnfhlcmhgbk"
  },
  {
    "MD", "paiickhoniddnnlhhdmhjcfemgkgfohn"
  },
  {
    "MC", "iloglofhibeghkfbocphifnfpccmplgd"
  },
  {
    "MN", "pclbpikpdcdondhappcgloeohjgammia"
  },
  {
    "ME", "dkjadbekoidbnlmaomlcjjgkofkechlo"
  },
  {
    "MS", "mknfcplgmgbfkklaiimdakgjbeonapeh"
  },
  {
    "MA", "pmbhpljpfciommdigfblnenpdiapdafl"
  },
  {
    "MZ", "gilieeicpdnkcjbohfhjhpmpjocapbko"
  },
  {
    "MM", "bbeoopklmfincipdlffbbphpjefmimmp"
  },
  {
    "NA", "paoffgbbehbibcihhmboiaebgojdnibj"
  },
  {
    "NR", "jpejbbflggaiaflclgomjcolpomjmhlh"
  },
  {
    "NP", "ohodaiianeochclnnobadfikohciggno"
  },
  {
    "NL", "choggjlbfndjppfiidbhmefapnlhcdhe"
  },
  {
    "NC", "apmipakgigaapfahiojgjgkfgcdekbpp"
  },
  {
    "NZ", "dlbokjgcdlhkgfeklggoncjhihaebnai"
  },
  {
    "NI", "jajkonoepahongnlnfbfmlnpnkjkchof"
  },
  {
    "NE", "mmhmpjfgnhibhfccegfohnibkpooppkn"
  },
  {
    "NG", "bhkddokohamnindobkmifljnpgijdjdh"
  },
  {
    "NU", "celbcocehclbnblfndjfjleagcbbpooc"
  },
  {
    "NF", "bcnnffpigdndbdohgifflckehcoofigc"
  },
  {
    "MK", "njlgnoebifbjpafbmnkkchknkinmeljm"
  },
  {
    "MP", "cpjjnbhhjohkkmkkplcfkobjfbjlildd"
  },
  {
    "NO", "ciibjdmjfejjghmnlonlihnjodfckfbo"
  },
  {
    "OM", "cobdmgempkofdfhgmbhfckemppmlbjob"
  },
  {
    "PK", "aiaabcbklimkipbpalfoaehfdebbainb"
  },
  {
    "PW", "ejlnmikcbnjpaaolkheodefhahiabjga"
  },
  {
    "PS", "iienfoenehmoepjgljgjdkenjedjfjol"
  },
  {
    "PA", "aafjalakdldginkbeobaiamnfphcdmko"
  },
  {
    "PG", "monkjbjmhjepdcaedlejhmjjjcjpiiaa"
  },
  {
    "PY", "aoidaoefdchfhdjfdffjnnlbfepfkadj"
  },
  {
    "PE", "pmbmbglgbofljclfopjkkompfgedgjhi"
  },
  {
    "PH", "ocmnmegmbhbfmdnjoppmlbhfcpmedacn"
  },
  {
    "PN", "ccpkbhegiebckfidhnoihgdmddhnmdfh"
  },
  {
    "PL", "feeklcgpaolphdiamjaolkkcpbeihkbh"
  },
  {
    "PT", "gchnahcajhccobheiedkgdpfboljkhge"
  },
  {
    "PR", "bpjdfagamlhoojajkeifbendedaikinl"
  },
  {
    "QA", "jicllaljbaldhopinkkegkfpmmnnhmbc"
  },
  {
    "RE", "aeglmpapdhfhdicbifhnmaoehffffmie"
  },
  {
    "RO", "jpapeieehcilkcfpljhopohigdhbnjck"
  },
  {
    "RU", "nfcegbjbohhjljcdogkmookngaiplbna"
  },
  {
    "RW", "djjoaejcadmjbgadeijpbokipgmolnih"
  },
  {
    "BL", "fjefhohmfmokjmnibamjnpiafikmmlef"
  },
  {
    "SH", "dpodaelfodkebmgmmdoecleopjggboln"
  },
  {
    "KN", "idmipdncpnfbfonogngaimigpbpnenpb"
  },
  {
    "LC", "lhlajkngiihbjjaakfgkencpppeahhfi"
  },
  {
    "MF", "hihpbgpfcelklhigbkfnbdgjmpbnabmo"
  },
  {
    "PM", "cpkbkgenaaododkijfnfmgmpekbcfjcg"
  },
  {
    "VC", "bnonnlpingklaggdalihppicgpaghpop"
  },
  {
    "WS", "jfckclnlfaekpfklphjagmjiphjcchmj"
  },
  {
    "SM", "lneikknijgnijfnpoahmfkefboallgin"
  },
  {
    "ST", "djlblammehomffbplemhekjeghekglpc"
  },
  {
    "SA", "gmhojjgbbfachddbgojljenplnhialka"
  },
  {
    "SN", "haalbaecaigldhgnjfmjbedegjipkdfb"
  },
  {
    "RS", "dlfdepidpnibdoblimabdmgpobophapn"
  },
  {
    "SC", "dmdapbmagehdijbdhbdbfjijgmcppjml"
  },
  {
    "SL", "piajfdpbabffhdlmpkaejndbdnohljfn"
  },
  {
    "SG", "jilipkheolgjanjhhhdmbaleiiblnepe"
  },
  {
    "SX", "igdomgnppdmcglgohoamnpegjelohlkj"
  },
  {
    "SK", "obponfmfefkaeehakbehbnnlcbebebhd"
  },
  {
    "SI", "dckjbnoilglapbgmniiagempimbaicmn"
  },
  {
    "SB", "mlbgbnccloeobccglpaachnaabgegcni"
  },
  {
    "SO", "hnfmhdkkmcgeppiiohpondjgibepgfeb"
  },
  {
    "ZA", "jadaiaonajphgnbamppeenldepoajgbf"
  },
  {
    "GS", "ghclfflogdfhnjciileceoofmhkneggp"
  },
  {
    "SS", "kkfngpdjfcddimfgkgibaccaoehjplkn"
  },
  {
    "ES", "ganmbmiebelpdlnohnabgkkocholelbp"
  },
  {
    "LK", "gmahgggkpliaoidcaknflpbgpehcjmhc"
  },
  {
    "SD", "dhcfofldcefkohnjcnfodlbiakgedidd"
  },
  {
    "SR", "khgbibnjdanhnoebnfjgpnfbkohdngph"
  },
  {
    "SJ", "kchkidfjkghdocdicfpmbckmjfgnlndb"
  },
  {
    "SE", "clncjboijmbkcjgkechfhalginbnplpp"
  },
  {
    "CH", "gnamhdlealpfbanappoephfdjeoehggd"
  },
  {
    "SY", "hnhakbhiamjojdoajhebemlajheokinm"
  },
  {
    "TW", "jejmkjlhckkijknapfhfoogakgoelhen"
  },
  {
    "TJ", "nfpgpnagpefhcijfnabpdejiiejplonp"
  },
  {
    "TZ", "jnlkpmlmfdipllbnjmjomkddplafclch"
  },
  {
    "TH", "mjkmkfbpiegjkbeolgpomaloeiiffodm"
  },
  {
    "TL", "kmdanbbapegbkpjkfdldmekconhnmmmo"
  },
  {
    "TG", "alinepjaedjagibhfjcejemabijbohmi"
  },
  {
    "TK", "bbobjkhpggphapdpcchkbklglkindkcc"
  },
  {
    "TO", "jdkdhebphdakbabdbgefjkdbdoagmdec"
  },
  {
    "TT", "nbmopmgpfmalleghhbkablkoamofibpk"
  },
  {
    "TN", "hgmkfpcpppjheoknjjfpajfmibkndjdf"
  },
  {
    "TR", "fahflofbglhemnakgdmillobeencekne"
  },
  {
    "TM", "fhbmmefncojhnjhbckninoliclloeeac"
  },
  {
    "TC", "hbiblijpgfgphhfoajjmcgdbhmobbfba"
  },
  {
    "TV", "kennokhomgefcfjjjhckbiidlhmkinca"
  },
  {
    "UG", "bolcbpmofcabjoflcmljongimpbpeagb"
  },
  {
    "UA", "enkpeojckjlmehbniegocfffdkanjhef"
  },
  {
    "AE", "ajdiilmgienedlgohldjicpcofnckdmn"
  },
  {
    "GB", "cdjnpippjnphaeahihhpafnneefcnnfh"
  },
  {
    "US", "kkjipiepeooghlclkedllogndmohhnhi"
  },
  {
    "UM", "ocikkcmgfagemkpbbkjlngjomkdobgpp"
  },
  {
    "UY", "cejbfkalcdepkoekifpidloabepihogd"
  },
  {
    "UZ", "chpbioaimigllimaalbibcmjjcfnbpid"
  },
  {
    "VU", "ogbkgicanbpgkemjooahemnoihlihonp"
  },
  {
    "VE", "okopabpainkagabcmkfnnchaakhimaoe"
  },
  {
    "VN", "jcffalbkohmmfjmgkdcphlimplejkmon"
  },
  {
    "VG", "jlfjphoakpnmhpldmdkdhekopbjmkljn"
  },
  {
    "VI", "infpagefbmdijbaigeagmldkjnjdhhfa"
  },
  {
    "WF", "hefgpgfflbaepfgbafaaadffchekggfg"
  },
  {
    "EH", "fjhkoeiglahhpcmgfpalgckcaoaifjkn"
  },
  {
    "YE", "cijopjkddpagbkjpnnbjcecfamjbkakp"
  },
  {
    "ZM", "inmfjchmafaondfnpgffefgbdmmfgenb"
  },
  {
    "ZW", "fmobbdfaoifmdjonfklmapdliabjdmjp"
  }
};

const std::set<std::string> kPurchaseIntentUserModelIds = {
  "jememeholcpjpoahinnlafoiaknnmfgl",
  "hfonhokmgmjionconfpknjfphfahdklo",
  "anlkmbkbgleadcacchhgdoecllpllknb",
  "imoolhehjnpebcjecoinphmohihmbccj",
  "kgnhcdjacgcanjnbdcmngdeoncckfmfh",
  "pmlmnjficamnkblapnohndlnhkkoaoco",
  "majdffglhcddbbanjnhfocagmmhobghd",
  "bhdlolcjjefaidodgffjhpbeeapabpgn",
  "pbanoihfljabneihobeplbciopfilajn",
  "cbdjliajiakicmdohhbjbgggacbjdnmn",
  "enadnicbppimlbpilkeaogmmfpennphn",
  "cpnhinhihfnhhmoknpbkcifgadjbcjnf",
  "ocegkjjbmlnibhfjmeiaidplhcbdhomd",
  "kklfafolbojbonkjgifmmkdmaaimminj",
  "jmneklmcodckmpipiekkfaokobhkflep",
  "llmikniomoddmmghodjfbncbidjlhhid",
  "aaoipmkakdldlippoaocoegnnfnpcokj",
  "megoieebjempmobckocciojgfhfmiejb",
  "ppkgobeickbpfmmkbhfgmgiloepdiagn",
  "ndfnmlonkimafoabeafnaignianecdhf",
  "apndmjdcfbhgfccccdmmeofpdgnlbbif",
  "lnbdfmpjjckjhnmahgdojnfnmdmpebfn",
  "demegfhfekncneocdlflandgegpcoofj",
  "jiodmgmkikfbkchgenlamoabbfnobnfh",
  "aeiffmlccgeaacefkfknodmnebanekbo",
  "hemccombjdaepjnhhdplhiaedaackooa",
  "dggplmjbjalpdgndkigojpikblaiflke",
  "jbibpedjodeigdoimlgpikphaabhdbie",
  "iefeaaegnnpiadjfoeifapjmflnbjehg",
  "bfdagcnfmfaidkkjcmfjmogiefoiglem",
  "kfimhlhdlhimaheficomcahhbaicoele",
  "fbpmbjccnaaeogogeldlomcmlhllgaje",
  "cpbmgmnfoakodmmpppabghhckchppneg",
  "gcikmigghagkligpileoekfjmohokjhm",
  "ahhbponhjohgifhjbjggafboffbimmmg",
  "fjgjapaemndekhnfeopeoeajfpmlgemo",
  "cfbbahiimladdkhblpkkokkmemlmkbhe",
  "oeneodeckioghmhokkmcbijfanjbanop",
  "cmknopomfihgdpjlnjhjkmogaooeoeic",
  "mmiflfidlgoehkhhkeodfdjpbkkjadgi",
  "gpaihfendegmjoffnpngjjhbipbioknd",
  "efpgpbmpbkhadlnpdnjigldeebgacopp",
  "ljfeoddejgcdofgnpgljaeiaemfimgej",
  "oahnhemdhgogkhgljdkhbficecbplmdf",
  "gbbfjnnpelockljikcmjkeodlaebjokm",
  "gfccfpdijajblfnkmddbflphiiibifik",
  "mjennfbimaafgcoekloojmbhnkophgni",
  "pnfogoijegjkepejdabehdfadbkpgoed",
  "cegiaceckhbagmmfcoeebeghiddghbkk",
  "efcmpmpbmaehimomnmonodlpnghelnfi",
  "hkgnnbjmfcelmehmphhbjigedknjobaa",
  "kignebofcmcgmjfiapilgdfkbekmkdmg",
  "clcghlkineckendfhkgdpkplofknofjo",
  "hmmoibmjgckbeejmcfflnngeacaklchb",
  "nopcbglolocphcdeikfkoeppkhijacij",
  "mjhnpmgafkmildljebajibghemlbffni",
  "mdopdmalncfakkimlojioichjbebcaip",
  "boeecnnjahpahhockgdcbdlaimpcflig",
  "hknminnkgcjafipipbbalkakppehkpjn",
  "iejekkikpddbbockoldagmfcdbffomfc",
  "kmfkbonhconlbieplamnikedgfbggail",
  "phihhhnelfclomhodhgalooldjgejgfl",
  "obihnhimgbeenjgfnlcdbfdgkkgeogdp",
  "gciaobanmdlfkegiikhgdoogegeghhch",
  "imnpbmpnmlmkjpgfnfeplikingjklpej",
  "ojfkdbfibflfejobeodhoepghgoghkii",
  "adnhangbagjlobdeicimblgcnafegpfb",
  "gncihmgakhljdlkadibldhdfnocfplci",
  "diacfpapelanfbkmehdpaaohmnkkngge",
  "jdigggiplmjlocdopbdmjibckpamigij",
  "npacefioaofgbofilfnhliofkefklipp",
  "lbiagcddmfapjfbebccolcahfppaimmo",
  "aogeahmaehgnkobkmnmkhkimdjpgfgen",
  "molpidmcmbmhbicckmbopbmiojddebke",
  "biobhkionbllnfljaapocfpdmhamedkf",
  "ecneelfoifpgnignhipdebhbkgcphmic",
  "bgifagoclclhhoflocdefiklgodpihog",
  "mhmfclafeemiphfebhnobinkplbmpocm",
  "mjaianjdadeiocokpoanbgjhegficcce",
  "jbjodokafbafhemocebljdblgnfajabi",
  "nchncleokkkkdfgbgmenkpkmhnlbibmg",
  "alebifccfdpcgpandngmalclpbjpaiak",
  "kaikhlldfkdjgddjdkangjobahokefeb",
  "dgkplhfdbkdogfblcghcfcgfalanhomi",
  "panlkcipneniikplpjnoddnhonjljbdp",
  "pibapallcmncjajdoamgdnmgcbefekgn",
  "ochemplgmlglilaflfjnmngpfmpmjgnb",
  "gjknekmnjalchmfjbecifkoohcmlolhp",
  "kbllnlfaaoonbliendlhmoleflgdekki",
  "keggdlgkcfmmopgnogknokiocjjieapm",
  "mfnbeofelbcabhloibhpklffiifjcdfl",
  "jdhabeecpolnjdiplbcpjlohmlgdjgjh",
  "ncldbgolkmlgfkoignkdjehiadnfmlid",
  "gcfgdkmegcjaceeofigbabmjjiojdgnb",
  "kheejcjggceghjdinbcklehfkobloboc",
  "fehpbhdbjpnaibhncpeedfkogiiajcne",
  "pkpmecljhbjbiicbnfcfgpgmpehldemm",
  "kfjeoeekjccibpockdjcdjbgagaaopdj",
  "ljhceaiogeejahjjblnfaaocgogchpkb",
  "llmmfofcojgcignfnaodhdhdhphhmfmf",
  "plpcpclbpkilccbegfbpediidmejaahc",
  "pofhnfhkonpjephlcjlmbjmlikiaddoc",
  "cljplhmgochlncaelcabfgeefebhmfnk",
  "kdkakecfnmlfifkhlekmfkmmkpgeckcl",
  "lanimmipljlbdnajkabaoiklnpcaoakp",
  "mhiehehcmljjmpibmpiadoeefnchmbdm",
  "oejhcomgcgcofdfchkdhkjohnioijofn",
  "fbmfelhaipnlicodibhjkmeafbcgpfnm",
  "blofecpleppfodonanffdbepbiijmklm",
  "fiodhmddlgkgajbhohfdmkliikidmaom",
  "gjkhegliajlngffafldbadcnpfegmkmb",
  "ncfbonfnhophngmkkihoieoclepddfhm",
  "ienmdlgalnmefnpjggommgdilkklopof",
  "lfgnglkpngeffaijiomlppnobpchhcgf",
  "gnkmfghehkoegoabhndflbdmfnlgeind",
  "jadlfgggcfdhicaoacokfpmccbmedkim",
  "bfhpiebgnciehokapeppcinalnibbnan",
  "dkghhhflbpfknidjbhlophapheggpahk",
  "pnokpaenadbgpjpmlnoamnmpjbjlfoaf",
  "clgbjhhcdihjgbomhpmfdjdiagejadja",
  "ehkeinmpkojiiacjohbalbnhloiaifig",
  "hehalbiboicjbbcfhckdfeijjjppdhij",
  "lhjcndbhldpnapjddfgohdcdmfibgpon",
  "pooflbdadogbmjmnnppfmklfjbmoblfa",
  "hkengofpokonjepdafjdeclejledncdj",
  "mdojkinfephdfhbfadcnnfcjfniefael",
  "alenpijagefjamgompebcjhbfhepnphh",
  "mnhglgpnnohpipdeinibpbnlnpledicf",
  "onhaidkdpiboaolhnaddeddfaabomchd",
  "aokfbnlokidoepkhilbmfdkdhajkpbli",
  "gnmaofjfninpeccldcmlkbinhhohmbck",
  "ncmdondkaofghlnhiabnfilafhmabong",
  "lapgbedoccnchodbgfmafpkkhlfmcehe",
  "dhmcaoadkmoejegjpjgkjhibioemkfni",
  "dadpenhnclbbkjfbkgkgecknfjggpbmm",
  "ggclalmmmmgjcoleeficgnnjkpgeinfd",
  "flocoipmnbpcodjfhmkjecjpbkcmkecp",
  "emckddclmcjoilbadmdjdakabpnkdkhk",
  "cpjafhooepmhnflmjabfeaiopfbljhpo",
  "chbeaiccoofemohdajloflfkblbgdiih",
  "dfmnoondmnbngeilibiicikjenjjeigi",
  "iobofpagkcicpcijjfmnghgppbghnpdo",
  "lcnaekpkllhpljanlibochejknjflodn",
  "dclpeadnefbjogjcamdglgmmbbgnjcob",
  "pjiglaefpchnekpbkbfngjnfhlcmhgbk",
  "paiickhoniddnnlhhdmhjcfemgkgfohn",
  "iloglofhibeghkfbocphifnfpccmplgd",
  "pclbpikpdcdondhappcgloeohjgammia",
  "dkjadbekoidbnlmaomlcjjgkofkechlo",
  "mknfcplgmgbfkklaiimdakgjbeonapeh",
  "pmbhpljpfciommdigfblnenpdiapdafl",
  "gilieeicpdnkcjbohfhjhpmpjocapbko",
  "bbeoopklmfincipdlffbbphpjefmimmp",
  "paoffgbbehbibcihhmboiaebgojdnibj",
  "jpejbbflggaiaflclgomjcolpomjmhlh",
  "ohodaiianeochclnnobadfikohciggno",
  "choggjlbfndjppfiidbhmefapnlhcdhe",
  "apmipakgigaapfahiojgjgkfgcdekbpp",
  "dlbokjgcdlhkgfeklggoncjhihaebnai",
  "jajkonoepahongnlnfbfmlnpnkjkchof",
  "mmhmpjfgnhibhfccegfohnibkpooppkn",
  "bhkddokohamnindobkmifljnpgijdjdh",
  "celbcocehclbnblfndjfjleagcbbpooc",
  "bcnnffpigdndbdohgifflckehcoofigc",
  "njlgnoebifbjpafbmnkkchknkinmeljm",
  "cpjjnbhhjohkkmkkplcfkobjfbjlildd",
  "ciibjdmjfejjghmnlonlihnjodfckfbo",
  "cobdmgempkofdfhgmbhfckemppmlbjob",
  "aiaabcbklimkipbpalfoaehfdebbainb",
  "ejlnmikcbnjpaaolkheodefhahiabjga",
  "iienfoenehmoepjgljgjdkenjedjfjol",
  "aafjalakdldginkbeobaiamnfphcdmko",
  "monkjbjmhjepdcaedlejhmjjjcjpiiaa",
  "aoidaoefdchfhdjfdffjnnlbfepfkadj",
  "pmbmbglgbofljclfopjkkompfgedgjhi",
  "ocmnmegmbhbfmdnjoppmlbhfcpmedacn",
  "ccpkbhegiebckfidhnoihgdmddhnmdfh",
  "feeklcgpaolphdiamjaolkkcpbeihkbh",
  "gchnahcajhccobheiedkgdpfboljkhge",
  "bpjdfagamlhoojajkeifbendedaikinl",
  "jicllaljbaldhopinkkegkfpmmnnhmbc",
  "aeglmpapdhfhdicbifhnmaoehffffmie",
  "jpapeieehcilkcfpljhopohigdhbnjck",
  "nfcegbjbohhjljcdogkmookngaiplbna",
  "djjoaejcadmjbgadeijpbokipgmolnih",
  "fjefhohmfmokjmnibamjnpiafikmmlef",
  "dpodaelfodkebmgmmdoecleopjggboln",
  "idmipdncpnfbfonogngaimigpbpnenpb",
  "lhlajkngiihbjjaakfgkencpppeahhfi",
  "hihpbgpfcelklhigbkfnbdgjmpbnabmo",
  "cpkbkgenaaododkijfnfmgmpekbcfjcg",
  "bnonnlpingklaggdalihppicgpaghpop",
  "jfckclnlfaekpfklphjagmjiphjcchmj",
  "lneikknijgnijfnpoahmfkefboallgin",
  "djlblammehomffbplemhekjeghekglpc",
  "gmhojjgbbfachddbgojljenplnhialka",
  "haalbaecaigldhgnjfmjbedegjipkdfb",
  "dlfdepidpnibdoblimabdmgpobophapn",
  "dmdapbmagehdijbdhbdbfjijgmcppjml",
  "piajfdpbabffhdlmpkaejndbdnohljfn",
  "jilipkheolgjanjhhhdmbaleiiblnepe",
  "igdomgnppdmcglgohoamnpegjelohlkj",
  "obponfmfefkaeehakbehbnnlcbebebhd",
  "dckjbnoilglapbgmniiagempimbaicmn",
  "mlbgbnccloeobccglpaachnaabgegcni",
  "hnfmhdkkmcgeppiiohpondjgibepgfeb",
  "jadaiaonajphgnbamppeenldepoajgbf",
  "ghclfflogdfhnjciileceoofmhkneggp",
  "kkfngpdjfcddimfgkgibaccaoehjplkn",
  "ganmbmiebelpdlnohnabgkkocholelbp",
  "gmahgggkpliaoidcaknflpbgpehcjmhc",
  "dhcfofldcefkohnjcnfodlbiakgedidd",
  "khgbibnjdanhnoebnfjgpnfbkohdngph",
  "kchkidfjkghdocdicfpmbckmjfgnlndb",
  "clncjboijmbkcjgkechfhalginbnplpp",
  "gnamhdlealpfbanappoephfdjeoehggd",
  "hnhakbhiamjojdoajhebemlajheokinm",
  "jejmkjlhckkijknapfhfoogakgoelhen",
  "nfpgpnagpefhcijfnabpdejiiejplonp",
  "jnlkpmlmfdipllbnjmjomkddplafclch",
  "mjkmkfbpiegjkbeolgpomaloeiiffodm",
  "kmdanbbapegbkpjkfdldmekconhnmmmo",
  "alinepjaedjagibhfjcejemabijbohmi",
  "bbobjkhpggphapdpcchkbklglkindkcc",
  "jdkdhebphdakbabdbgefjkdbdoagmdec",
  "nbmopmgpfmalleghhbkablkoamofibpk",
  "hgmkfpcpppjheoknjjfpajfmibkndjdf",
  "fahflofbglhemnakgdmillobeencekne",
  "fhbmmefncojhnjhbckninoliclloeeac",
  "hbiblijpgfgphhfoajjmcgdbhmobbfba",
  "kennokhomgefcfjjjhckbiidlhmkinca",
  "bolcbpmofcabjoflcmljongimpbpeagb",
  "enkpeojckjlmehbniegocfffdkanjhef",
  "ajdiilmgienedlgohldjicpcofnckdmn",
  "cdjnpippjnphaeahihhpafnneefcnnfh",
  "kkjipiepeooghlclkedllogndmohhnhi",
  "ocikkcmgfagemkpbbkjlngjomkdobgpp",
  "cejbfkalcdepkoekifpidloabepihogd",
  "chpbioaimigllimaalbibcmjjcfnbpid",
  "ogbkgicanbpgkemjooahemnoihlihonp",
  "okopabpainkagabcmkfnnchaakhimaoe",
  "jcffalbkohmmfjmgkdcphlimplejkmon",
  "jlfjphoakpnmhpldmdkdhekopbjmkljn",
  "infpagefbmdijbaigeagmldkjnjdhhfa",
  "hefgpgfflbaepfgbafaaadffchekggfg",
  "fjhkoeiglahhpcmgfpalgckcaoaifjkn",
  "cijopjkddpagbkjpnnbjcecfamjbkakp",
  "inmfjchmafaondfnpgffefgbdmmfgenb",
  "fmobbdfaoifmdjonfklmapdliabjdmjp"
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_CLASSIFICATION_PURCHASE_INTENT_CLASSIFIER_PURCHASE_INTENT_CLASSIFIER_USER_MODELS_H_  // NOLINT
