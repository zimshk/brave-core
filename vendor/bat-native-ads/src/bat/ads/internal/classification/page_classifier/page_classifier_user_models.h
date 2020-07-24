/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_CLASSIFICATION_PAGE_CLASSIFIER_PAGE_CLASSIFIER_USER_MODELS_H_  // NOLINT
#define BAT_ADS_INTERNAL_CLASSIFICATION_PAGE_CLASSIFIER_PAGE_CLASSIFIER_USER_MODELS_H_  // NOLINT

#include <map>
#include <set>
#include <string>

namespace ads {

const std::set<std::string> kPageClassificationUserModelIds = {
  "ijmgabghpbflfadffhpmjklamndnonha",
  "hddanjaanmjbdklklpldpgpmbdmaiihb",
  "blcjdmhlkddhompnlbjlpccpelipihem",
  "pecokcgeeiabdlkfkfjpmfldfhhjlmom",
  "pgkommhmfkkkfbbcnnfhlkagbodoimjm",
  "emopjfcnkbjfedjbfgajdajnkkfekcbl",
  "hfiknbegiiiigegdgpcgekhdlpdmladb",
  "onjbjcnjheabeghbflckfekjnnjgfabn",
  "ghgfdmhmhifphaokjfplffppmlfchofm",
  "mbcmenffnlanegglgpdgbmmldfpclnkm",
  "clemenahenllbgeilmkllcpfcfhloebp",
  "cmggjadifnmfdfeohgomnaggonihmbli",
  "fabdaiigiipinlpgkkakcffbhkmfnfek",
  "cgpdnipmfmbfceokoadgbliclnoeddef",
  "hjfgajachhnlgjfjgbjbehgcoecfigap",
  "keghklelbpkaiogcinbjnnjideedfmdd",
  "lldcgicjgpomllohjncpcjkijjklooji",
  "ogmkkmobemflinkielcngdanaojkamcc",
  "ligbalgipjnajannojamijenlackcioc",
  "bkicjkclpdihdjbpajnegckopabcebff",
  "dafhhkffcifanfgpjlgejcahkidpbnfj",
  "ngnfehahlmgkoclalhldboigojplccbl",
  "hkfnnljkbmadknefddjfligobbiagmea",
  "igdpeoeohlgknkdnjhbijpfeenfiepdc",
  "mdacdgffhlidpdmhckokeoajhojeibmp",
  "keahfifnfebhoaaghffigjnhgbkkaibd",
  "fllhkadnpidionapphjicdkfdiloghad",
  "eakppbikiomjdnligoccikcdipojiljk",
  "ddekfkhnjcpbankekbelkeekibbhmgnh",
  "oblfikajhadjnmjiihdchdfdcfehlbpj",
  "obdagonejiaflgbifdloghkllgdjpcdj",
  "apmkijnjnhdabkckmkkejgdnbgdglocb",
  "gdmbpbmoiogajaogpajfhonmlepcdokn",
  "amedpgedagedjlkgjajebgecfkldmdfa",
  "ncjeinjdknabbgmaijmnipndbggmchff",
  "nllaodpgkekajbabhjphhekenlnlpdmd",
  "klifniioldbebiedppmbobpdiombacge",
  "aoljgchlinejchjbbkicamhfdapghahp",
  "opoleacilplnkhobipjcihpdoklpnjkk",
  "jginkacioplimdjobccplmgiphpjjigl",
  "emgmepnebbddgnkhfmhdhmjifkglkamo",
  "halbpcgpgcafebkldcfhllomekophick",
  "onmakacikbbnhmanodpjhijljadlpbda",
  "bjhkalknmdllcnkcjdjncplkbbeigklb",
  "jamflccjbegjmghgaljipcjjbipgojbn",
  "gfoibbmiaikelajlipoffiklghlbapjf",
  "djokgcimofealcnfijnlfdnfajpdjcfg",
  "hbejpnagkgeeohiojniljejpdpojmfdp",
  "anhpkncejedojfopbigplmbfmbhkagao",
  "ejioajnkmjfjfbbanmgfbagjbdlfodge",
  "hlipecdabdcghhdkhfmhiclaobjjmhbo",
  "eclclcmhpefndfimkgjknaenojpdffjp",
  "aefhgfnampgebnpchhfkaoaiijpmhcca",
  "ebgmbleidclecpicaccgpjdhndholiel",
  "mdpcebhdjplkegddkiodmbjcmlnhbpog",
  "hpkelamfnimiapfbeeelpfkhkgfoejil",
  "khgpojhhoikmhodflkppdcakhbkaojpi",
  "gffjpkbdngpbfkflpnoodjfkpelbappk",
  "pkmkhkpglegjekkfabaelkbgkfegnmde",
  "emhbebmifclalgbdpodobmckfehlkhfp",
  "cmkgcbedakcdopgjdhbbpjjaodjcdbdp",
  "ifbdofecjcadpnkokmdfahhjadcppmko",
  "hoeihggfhgnfpdnaeocfpmoelcenhfla",
  "gbmolmcnbhegkhjhbhjehcpbjonjlgfg",
  "fioklomfllflofcdiklpgabemclgkkdh",
  "oiihbhoknlbjonghmcplpbpkcdeinlfg",
  "nchaailfhkbnlnaobgjmoamdfnclhdoo",
  "fbdjobfnceggaokdnlebbaplnhednlhl",
  "nkajjfpapgfhlcpmmoipafbfnnmojaep",
  "dhhkjdedjghadoekfklpheblplmlpdec",
  "ijaiihoedhaocihjjkfjnhfhbceekdkg",
  "eociikjclgmjinkgeoehleofopogehja",
  "ncnmgkcadooabjhgjlkkdipdnfokpjnm",
  "jdfafcdnmjeadcohbmjeeijgheobldng",
  "jknflnmanienedkoeoginjmbpmkpclki",
  "nggdckgfnooehkpnmjdodbknekmhcdeg",
  "mhobnohaonkecggnifnffofnihbakjic",
  "hhejjhncnckfmpkpkhkbknekhkepcill",
  "ljllkgialkdmamlacenmjkhjmimndfil",
  "dhigelnhdnmhffomicjdmdhefhedgijm",
  "jcfelbpkigpapilbogpidbfdffgdfafe",
  "ookcfboifajbeojnnebiaoplifgiinof",
  "njimokpabbaelbbpohoflcbjhdgonbmf",
  "danmahhfdmncbiipbefmjdkembceajdk",
  "lcahllbcfbhghpjjocdhmilokfbpbekn",
  "jbhiacghlejpbieldkdfkgenhnolndlf",
  "hfboaaehpnfpnpompagbamoknlnladfn",
  "cppbcboljlmfdgeehadijemhkifhcpnl",
  "knnpciecjoakhokllbgioaceglldlgan",
  "chnbfebpflegknnjiikofmnebcbphead",
  "hkfkdejbdionnjdhgfhpkcmknmamddde",
  "nnbaaidlgckbmfdlnioepikbcmjmbadb",
  "dedchpogdooaakplmpjobpkiggalnlif",
  "alaghdpgppakjfjcipokbaempndnglfk",
  "copfpkggfedomijbmceepmahananponb",
  "ljambfnmibabkhcpgppfblodipceahab",
  "lflklgkjbemnncejeanindnikiaicpod",
  "lkcfaihllkinjhmdjgaccjhgdobabpbj",
  "anikcbbbhcobgockdcemaboadbdcnhlg",
  "gaompbafbaolhddgoafjhkgmnjpjpbip",
  "pppjaeenalohmnhjpemhdkkkghfddbfp",
  "papnfbjjblebaaeenodbmllepiijfmhn",
  "jmakhmnfhlhioagnmgnakhigadgkpkcl",
  "gakleannelcniohpkolmbgfpnkjlbnll",
  "lmpaafjmphnclpkfcejgjbnieahkgemg",
  "fehmnfinbijjdacjgeofhojfdjhbehic",
  "aadlcogkfhaficfijoolhlajkngeecea",
  "edcpinkoiknmjafcdpolkkeiieddmbab",
  "ooncphbdmekmhldbojgoboebnongkpic",
  "kemfolmmdooeepfhbpiemnnekfjlbnnd",
  "mihcpmkclenpfgielcipmdbcfpncfojc",
  "jpleaamlgnfhfemdcfmbknnhcbfnglkh",
  "gbigbbblbdmfhbpobcliehihdedicjfl",
  "fnmjhgcaloniglpngailbaillhbenela",
  "fgicknpghikljlipmfibgghcdikjfjfj",
  "nodfcenkjehpafmbmgnapoieilnoijap",
  "dghnlalnaoekcligakadcmmioaieangj",
  "fbfeebiglbpbmgbefgmijdbcchmdfdhm",
  "gdkeabmpgllapbjgifgfmlfelpdlkapj",
  "fnhldinjahkdbngcnjfcmidhpjedinbg",
  "aegokocmijocdgiddgjbjkdfiheijfpl",
  "amkpggbpieoafkbmkijjnefikhjjfogn",
  "adccmiokahgjhdbmhldbnkkplgcgfkpp",
  "ghikcfknmlkdjiiljfpgpmcfjinpollk",
  "hinecgnhkigghpncjnflaokadaclcfpm",
  "blaocfojieebnkolagngecdhnipocicj",
  "aijecnhpjljblhnogamehknbmljlbfgn",
  "fikmpfipjepnnhiolongfjimedlhaemk",
  "ikpplkdenofcphgejneekjmhepajgopf",
  "ndlciiakjgfcefimfjlfcjgohlgidpnc",
  "nlabdknfkecjaechkekdlkgnapljpfla",
  "piebpodmljdloocefikhekfjajloneij",
  "hffipkehifobjlkdjagndofmpjnpkgje",
  "nigmjcnboijpcoikglccmoncigioojpa",
  "inkmdnaeojfdngbdkbinoinflfahcjfc",
  "golaphmkhjkdmcakpigbjhneagiapkfh",
  "kcmiboiihhehobbffjebhgadbalknboh",
  "cmomlghkjichjgbkakaoenfenincefnj",
  "mfaajikelgfodgcgnldapbgjdbncmibc",
  "gndfhmmkadfdhmchhljmcdhlicdmmlbn",
  "pdgppejghdoknllcnfikoaloobopajjo",
  "djmefhmnkffabdodgcfjmgffpindaaak",
  "bdepmnbdekgdgjimffimkfeoggmnlbbf",
  "mogllbjhcnfhcejalaikleeogjmmfkdm",
  "gnhdcgmlejfbcccdjdhjalacjcimlkjh",
  "jifgjineejhedlmjnkcijoincbhelicp",
  "doclofgiadjildnifgkajdlpngijbpop",
  "mgdaglmilmjenimbkdmneckfbphfllic",
  "ahiocclicnhmiobhocikfdamfccbehhn",
  "aondicpcneldjpbfemaimbpomgaepjhg",
  "ccmmjlklhnoojaganaecljeecenhafok",
  "khbhchcpljcejlmijghlabpcmlkkfnid",
  "jpgndiehmchkacbfggdgkoohioocdhbp",
  "nbmbpelgpalcgdghkeafabljjbhmalhf",
  "nonmahhknjgpnoamcdihefcbpdioimbh",
  "olopfkdcklijkianjbegdegilmhdgpcj",
  "jllmphacilbjnfngcojfgmiimipclfbm",
  "hkeoedmbihkkglaobeembiogeofffpop",
  "ijgcgakmmgjaladckdppemjgdnjlcgpo",
  "liddcpbnodlgenmbmmfchepoebgfondk",
  "kcoilhabhhnfdakenmhddnhngngggcmp",
  "gjinficpofcocgaaogaiimhacbfkmjmj",
  "hhliclmbfpdlpkdhmpkleicjnemheeea",
  "kpdcfihnokkbialolpedfamclbdlgopi",
  "nhbpjehmiofogaicflcjhcfdmmkgbohp",
  "mmmembcojnkgfclnogjfeeiicmiijcnk",
  "ldjaelhegioonaebajlgfnhcipdajfeo",
  "fnokionjijmckgggmhogifnocpabdafk",
  "ohcnbpfpchlcjchnljcdjebcjdbneecj",
  "pbegjfplhidokoonohceljofcpbojglg",
  "jaggpekahffhnhhchdkpnigfmdlhenpo",
  "jephmoboccidmbemhjckbcagagijgcef",
  "mbhiljiiffkobikkoechkpeaopagfhep",
  "pbjakpdfjkmcajeobebemnjglbjiniln",
  "bfljdbgfmdjgbomhiaoeoleidbfcmmpn",
  "fmiofedgokpciaklgakbnminmmkocgnd",
  "gpfmbdepojhpjlaelmnlbgginpgnbmfd",
  "mhdpccgjfkfkdbbpapbgcahhknmbdnjn",
  "eahefjeohmofagkliaddkmokbecfhclm",
  "gjigddoamjemfcahionjikmlfijoiecf",
  "jhnklldjooclfmgpkipaemehnngabckf",
  "fjfbodkpnkomodlcanacakhcfmjjgkdf",
  "bncbapkadghlbephbogcmomlecfmdhnb",
  "dhlnknppkgfgehmmipicnlplhjgpnmnh"
};

const std::map<std::string, std::string> kPageClassificationLanguageCodes = {
  {
    "ab", "ijmgabghpbflfadffhpmjklamndnonha"
  },
  {
    "aa", "hddanjaanmjbdklklpldpgpmbdmaiihb"
  },
  {
    "af", "blcjdmhlkddhompnlbjlpccpelipihem"
  },
  {
    "ak", "pecokcgeeiabdlkfkfjpmfldfhhjlmom"
  },
  {
    "sq", "pgkommhmfkkkfbbcnnfhlkagbodoimjm"
  },
  {
    "am", "emopjfcnkbjfedjbfgajdajnkkfekcbl"
  },
  {
    "ar", "hfiknbegiiiigegdgpcgekhdlpdmladb"
  },
  {
    "an", "onjbjcnjheabeghbflckfekjnnjgfabn"
  },
  {
    "hy", "ghgfdmhmhifphaokjfplffppmlfchofm"
  },
  {
    "as", "mbcmenffnlanegglgpdgbmmldfpclnkm"
  },
  {
    "av", "clemenahenllbgeilmkllcpfcfhloebp"
  },
  {
    "ae", "cmggjadifnmfdfeohgomnaggonihmbli"
  },
  {
    "ay", "fabdaiigiipinlpgkkakcffbhkmfnfek"
  },
  {
    "az", "cgpdnipmfmbfceokoadgbliclnoeddef"
  },
  {
    "bm", "hjfgajachhnlgjfjgbjbehgcoecfigap"
  },
  {
    "ba", "keghklelbpkaiogcinbjnnjideedfmdd"
  },
  {
    "eu", "lldcgicjgpomllohjncpcjkijjklooji"
  },
  {
    "be", "ogmkkmobemflinkielcngdanaojkamcc"
  },
  {
    "bn", "ligbalgipjnajannojamijenlackcioc"
  },
  {
    "bh", "bkicjkclpdihdjbpajnegckopabcebff"
  },
  {
    "bi", "dafhhkffcifanfgpjlgejcahkidpbnfj"
  },
  {
    "bs", "ngnfehahlmgkoclalhldboigojplccbl"
  },
  {
    "br", "hkfnnljkbmadknefddjfligobbiagmea"
  },
  {
    "bg", "igdpeoeohlgknkdnjhbijpfeenfiepdc"
  },
  {
    "my", "mdacdgffhlidpdmhckokeoajhojeibmp"
  },
  {
    "ca", "keahfifnfebhoaaghffigjnhgbkkaibd"
  },
  {
    "ch", "fllhkadnpidionapphjicdkfdiloghad"
  },
  {
    "ce", "eakppbikiomjdnligoccikcdipojiljk"
  },
  {
    "ny", "ddekfkhnjcpbankekbelkeekibbhmgnh"
  },
  {
    "zh", "oblfikajhadjnmjiihdchdfdcfehlbpj"
  },
  {
    "cv", "obdagonejiaflgbifdloghkllgdjpcdj"
  },
  {
    "kw", "apmkijnjnhdabkckmkkejgdnbgdglocb"
  },
  {
    "co", "gdmbpbmoiogajaogpajfhonmlepcdokn"
  },
  {
    "cr", "amedpgedagedjlkgjajebgecfkldmdfa"
  },
  {
    "hr", "ncjeinjdknabbgmaijmnipndbggmchff"
  },
  {
    "cs", "nllaodpgkekajbabhjphhekenlnlpdmd"
  },
  {
    "da", "klifniioldbebiedppmbobpdiombacge"
  },
  {
    "dv", "aoljgchlinejchjbbkicamhfdapghahp"
  },
  {
    "nl", "opoleacilplnkhobipjcihpdoklpnjkk"
  },
  {
    "dz", "jginkacioplimdjobccplmgiphpjjigl"
  },
  {
    "en", "emgmepnebbddgnkhfmhdhmjifkglkamo"
  },
  {
    "eo", "halbpcgpgcafebkldcfhllomekophick"
  },
  {
    "et", "onmakacikbbnhmanodpjhijljadlpbda"
  },
  {
    "ee", "bjhkalknmdllcnkcjdjncplkbbeigklb"
  },
  {
    "fo", "jamflccjbegjmghgaljipcjjbipgojbn"
  },
  {
    "fj", "gfoibbmiaikelajlipoffiklghlbapjf"
  },
  {
    "fi", "djokgcimofealcnfijnlfdnfajpdjcfg"
  },
  {
    "fr", "hbejpnagkgeeohiojniljejpdpojmfdp"
  },
  {
    "ff", "anhpkncejedojfopbigplmbfmbhkagao"
  },
  {
    "gl", "ejioajnkmjfjfbbanmgfbagjbdlfodge"
  },
  {
    "ka", "hlipecdabdcghhdkhfmhiclaobjjmhbo"
  },
  {
    "de", "eclclcmhpefndfimkgjknaenojpdffjp"
  },
  {
    "el", "aefhgfnampgebnpchhfkaoaiijpmhcca"
  },
  {
    "gn", "ebgmbleidclecpicaccgpjdhndholiel"
  },
  {
    "gu", "mdpcebhdjplkegddkiodmbjcmlnhbpog"
  },
  {
    "ht", "hpkelamfnimiapfbeeelpfkhkgfoejil"
  },
  {
    "ha", "khgpojhhoikmhodflkppdcakhbkaojpi"
  },
  {
    "he", "gffjpkbdngpbfkflpnoodjfkpelbappk"
  },
  {
    "hz", "pkmkhkpglegjekkfabaelkbgkfegnmde"
  },
  {
    "hi", "emhbebmifclalgbdpodobmckfehlkhfp"
  },
  {
    "ho", "cmkgcbedakcdopgjdhbbpjjaodjcdbdp"
  },
  {
    "hu", "ifbdofecjcadpnkokmdfahhjadcppmko"
  },
  {
    "ia", "hoeihggfhgnfpdnaeocfpmoelcenhfla"
  },
  {
    "id", "gbmolmcnbhegkhjhbhjehcpbjonjlgfg"
  },
  {
    "ie", "fioklomfllflofcdiklpgabemclgkkdh"
  },
  {
    "ga", "oiihbhoknlbjonghmcplpbpkcdeinlfg"
  },
  {
    "ig", "nchaailfhkbnlnaobgjmoamdfnclhdoo"
  },
  {
    "ik", "fbdjobfnceggaokdnlebbaplnhednlhl"
  },
  {
    "io", "nkajjfpapgfhlcpmmoipafbfnnmojaep"
  },
  {
    "is", "dhhkjdedjghadoekfklpheblplmlpdec"
  },
  {
    "it", "ijaiihoedhaocihjjkfjnhfhbceekdkg"
  },
  {
    "iu", "eociikjclgmjinkgeoehleofopogehja"
  },
  {
    "ja", "ncnmgkcadooabjhgjlkkdipdnfokpjnm"
  },
  {
    "jv", "jdfafcdnmjeadcohbmjeeijgheobldng"
  },
  {
    "kl", "jknflnmanienedkoeoginjmbpmkpclki"
  },
  {
    "kn", "nggdckgfnooehkpnmjdodbknekmhcdeg"
  },
  {
    "kr", "mhobnohaonkecggnifnffofnihbakjic"
  },
  {
    "ks", "hhejjhncnckfmpkpkhkbknekhkepcill"
  },
  {
    "kk", "ljllkgialkdmamlacenmjkhjmimndfil"
  },
  {
    "km", "dhigelnhdnmhffomicjdmdhefhedgijm"
  },
  {
    "ki", "jcfelbpkigpapilbogpidbfdffgdfafe"
  },
  {
    "rw", "ookcfboifajbeojnnebiaoplifgiinof"
  },
  {
    "ky", "njimokpabbaelbbpohoflcbjhdgonbmf"
  },
  {
    "kv", "danmahhfdmncbiipbefmjdkembceajdk"
  },
  {
    "kg", "lcahllbcfbhghpjjocdhmilokfbpbekn"
  },
  {
    "ko", "jbhiacghlejpbieldkdfkgenhnolndlf"
  },
  {
    "ku", "hfboaaehpnfpnpompagbamoknlnladfn"
  },
  {
    "kj", "cppbcboljlmfdgeehadijemhkifhcpnl"
  },
  {
    "la", "knnpciecjoakhokllbgioaceglldlgan"
  },
  {
    "lb", "chnbfebpflegknnjiikofmnebcbphead"
  },
  {
    "lg", "hkfkdejbdionnjdhgfhpkcmknmamddde"
  },
  {
    "li", "nnbaaidlgckbmfdlnioepikbcmjmbadb"
  },
  {
    "ln", "dedchpogdooaakplmpjobpkiggalnlif"
  },
  {
    "lo", "alaghdpgppakjfjcipokbaempndnglfk"
  },
  {
    "lt", "copfpkggfedomijbmceepmahananponb"
  },
  {
    "lu", "ljambfnmibabkhcpgppfblodipceahab"
  },
  {
    "lv", "lflklgkjbemnncejeanindnikiaicpod"
  },
  {
    "gv", "lkcfaihllkinjhmdjgaccjhgdobabpbj"
  },
  {
    "mk", "anikcbbbhcobgockdcemaboadbdcnhlg"
  },
  {
    "mg", "gaompbafbaolhddgoafjhkgmnjpjpbip"
  },
  {
    "ms", "pppjaeenalohmnhjpemhdkkkghfddbfp"
  },
  {
    "ml", "papnfbjjblebaaeenodbmllepiijfmhn"
  },
  {
    "mt", "jmakhmnfhlhioagnmgnakhigadgkpkcl"
  },
  {
    "mi", "gakleannelcniohpkolmbgfpnkjlbnll"
  },
  {
    "mr", "lmpaafjmphnclpkfcejgjbnieahkgemg"
  },
  {
    "mh", "fehmnfinbijjdacjgeofhojfdjhbehic"
  },
  {
    "mn", "aadlcogkfhaficfijoolhlajkngeecea"
  },
  {
    "na", "edcpinkoiknmjafcdpolkkeiieddmbab"
  },
  {
    "nv", "ooncphbdmekmhldbojgoboebnongkpic"
  },
  {
    "nd", "kemfolmmdooeepfhbpiemnnekfjlbnnd"
  },
  {
    "ne", "mihcpmkclenpfgielcipmdbcfpncfojc"
  },
  {
    "ng", "jpleaamlgnfhfemdcfmbknnhcbfnglkh"
  },
  {
    "nb", "gbigbbblbdmfhbpobcliehihdedicjfl"
  },
  {
    "nn", "fnmjhgcaloniglpngailbaillhbenela"
  },
  {
    "no", "fgicknpghikljlipmfibgghcdikjfjfj"
  },
  {
    "ii", "nodfcenkjehpafmbmgnapoieilnoijap"
  },
  {
    "nr", "dghnlalnaoekcligakadcmmioaieangj"
  },
  {
    "oc", "fbfeebiglbpbmgbefgmijdbcchmdfdhm"
  },
  {
    "oj", "gdkeabmpgllapbjgifgfmlfelpdlkapj"
  },
  {
    "cu", "fnhldinjahkdbngcnjfcmidhpjedinbg"
  },
  {
    "om", "aegokocmijocdgiddgjbjkdfiheijfpl"
  },
  {
    "or", "amkpggbpieoafkbmkijjnefikhjjfogn"
  },
  {
    "os", "adccmiokahgjhdbmhldbnkkplgcgfkpp"
  },
  {
    "pa", "ghikcfknmlkdjiiljfpgpmcfjinpollk"
  },
  {
    "pi", "hinecgnhkigghpncjnflaokadaclcfpm"
  },
  {
    "fa", "blaocfojieebnkolagngecdhnipocicj"
  },
  {
    "pl", "aijecnhpjljblhnogamehknbmljlbfgn"
  },
  {
    "ps", "fikmpfipjepnnhiolongfjimedlhaemk"
  },
  {
    "pt", "ikpplkdenofcphgejneekjmhepajgopf"
  },
  {
    "qu", "ndlciiakjgfcefimfjlfcjgohlgidpnc"
  },
  {
    "rm", "nlabdknfkecjaechkekdlkgnapljpfla"
  },
  {
    "rn", "piebpodmljdloocefikhekfjajloneij"
  },
  {
    "ro", "hffipkehifobjlkdjagndofmpjnpkgje"
  },
  {
    "ru", "nigmjcnboijpcoikglccmoncigioojpa"
  },
  {
    "sa", "inkmdnaeojfdngbdkbinoinflfahcjfc"
  },
  {
    "sc", "golaphmkhjkdmcakpigbjhneagiapkfh"
  },
  {
    "sd", "kcmiboiihhehobbffjebhgadbalknboh"
  },
  {
    "se", "cmomlghkjichjgbkakaoenfenincefnj"
  },
  {
    "sm", "mfaajikelgfodgcgnldapbgjdbncmibc"
  },
  {
    "sg", "gndfhmmkadfdhmchhljmcdhlicdmmlbn"
  },
  {
    "sr", "pdgppejghdoknllcnfikoaloobopajjo"
  },
  {
    "gd", "djmefhmnkffabdodgcfjmgffpindaaak"
  },
  {
    "sn", "bdepmnbdekgdgjimffimkfeoggmnlbbf"
  },
  {
    "si", "mogllbjhcnfhcejalaikleeogjmmfkdm"
  },
  {
    "sk", "gnhdcgmlejfbcccdjdhjalacjcimlkjh"
  },
  {
    "sl", "jifgjineejhedlmjnkcijoincbhelicp"
  },
  {
    "so", "doclofgiadjildnifgkajdlpngijbpop"
  },
  {
    "st", "mgdaglmilmjenimbkdmneckfbphfllic"
  },
  {
    "es", "ahiocclicnhmiobhocikfdamfccbehhn"
  },
  {
    "su", "aondicpcneldjpbfemaimbpomgaepjhg"
  },
  {
    "sw", "ccmmjlklhnoojaganaecljeecenhafok"
  },
  {
    "ss", "khbhchcpljcejlmijghlabpcmlkkfnid"
  },
  {
    "sv", "jpgndiehmchkacbfggdgkoohioocdhbp"
  },
  {
    "ta", "nbmbpelgpalcgdghkeafabljjbhmalhf"
  },
  {
    "te", "nonmahhknjgpnoamcdihefcbpdioimbh"
  },
  {
    "tg", "olopfkdcklijkianjbegdegilmhdgpcj"
  },
  {
    "th", "jllmphacilbjnfngcojfgmiimipclfbm"
  },
  {
    "ti", "hkeoedmbihkkglaobeembiogeofffpop"
  },
  {
    "bo", "ijgcgakmmgjaladckdppemjgdnjlcgpo"
  },
  {
    "tk", "liddcpbnodlgenmbmmfchepoebgfondk"
  },
  {
    "tl", "kcoilhabhhnfdakenmhddnhngngggcmp"
  },
  {
    "tn", "gjinficpofcocgaaogaiimhacbfkmjmj"
  },
  {
    "to", "hhliclmbfpdlpkdhmpkleicjnemheeea"
  },
  {
    "tr", "kpdcfihnokkbialolpedfamclbdlgopi"
  },
  {
    "ts", "nhbpjehmiofogaicflcjhcfdmmkgbohp"
  },
  {
    "tt", "mmmembcojnkgfclnogjfeeiicmiijcnk"
  },
  {
    "tw", "ldjaelhegioonaebajlgfnhcipdajfeo"
  },
  {
    "ty", "fnokionjijmckgggmhogifnocpabdafk"
  },
  {
    "ug", "ohcnbpfpchlcjchnljcdjebcjdbneecj"
  },
  {
    "uk", "pbegjfplhidokoonohceljofcpbojglg"
  },
  {
    "ur", "jaggpekahffhnhhchdkpnigfmdlhenpo"
  },
  {
    "uz", "jephmoboccidmbemhjckbcagagijgcef"
  },
  {
    "ve", "mbhiljiiffkobikkoechkpeaopagfhep"
  },
  {
    "vi", "pbjakpdfjkmcajeobebemnjglbjiniln"
  },
  {
    "vo", "bfljdbgfmdjgbomhiaoeoleidbfcmmpn"
  },
  {
    "wa", "fmiofedgokpciaklgakbnminmmkocgnd"
  },
  {
    "cy", "gpfmbdepojhpjlaelmnlbgginpgnbmfd"
  },
  {
    "wo", "mhdpccgjfkfkdbbpapbgcahhknmbdnjn"
  },
  {
    "fy", "eahefjeohmofagkliaddkmokbecfhclm"
  },
  {
    "xh", "gjigddoamjemfcahionjikmlfijoiecf"
  },
  {
    "yi", "jhnklldjooclfmgpkipaemehnngabckf"
  },
  {
    "yo", "fjfbodkpnkomodlcanacakhcfmjjgkdf"
  },
  {
    "za", "bncbapkadghlbephbogcmomlecfmdhnb"
  },
  {
    "zu", "dhlnknppkgfgehmmipicnlplhjgpnmnh"
  }
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_CLASSIFICATION_PAGE_CLASSIFIER_PAGE_CLASSIFIER_USER_MODELS_H_  // NOLINT
