#!/usr/bin/env ruby
require File.dirname(__FILE__) + '/ext/wikitext'
require 'benchmark'
require 'iconv'

short_slab_of_ASCII_text  = 'Lorem ipsum dolor sit amet, consectetuer adipiscing elit.'

short_slab_of_UTF8_text   = 'Lór€m ïpsûm dólór sït àm€t, cóñs€ct€tû€r àdïpïscïñg €lït.'

longer_slab_of_ASCII_text = <<SLAB
Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Sed molestie ipsum at sapien. Nullam tempus, risus euismod rutrum mollis, pede lectus ullamcorper nisi, et auctor arcu velit ut ligula. Donec at felis. Aenean vulputate. Sed sed diam non metus varius pellentesque. Nulla ac nibh et metus pharetra convallis. Phasellus libero. Nunc viverra tincidunt dolor. Duis risus. Proin semper rhoncus quam. Sed vestibulum molestie lectus. Suspendisse sit amet tellus ac tortor tincidunt commodo. Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Duis venenatis imperdiet eros. Duis tempor elementum libero. Morbi id felis. In tincidunt consectetuer erat. Praesent varius pulvinar lectus.

Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Phasellus cursus turpis in metus. Integer tristique diam a nunc. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Curabitur sed sapien in lectus auctor pellentesque. Nam quis leo id lacus fermentum sagittis. Duis semper odio tincidunt est. Fusce convallis, metus id faucibus posuere, tortor urna viverra nisl, vel rhoncus mi urna in justo. Fusce dignissim lorem eget augue. Morbi pede nibh, elementum vitae, convallis sit amet, feugiat eu, eros. Donec blandit est sed augue. Aliquam vulputate, leo a tempor suscipit, lectus felis semper urna, sed luctus pede neque eu tortor. Nulla facilisi.

Ut eget risus ut erat volutpat ultrices. Sed nec sapien ultrices nunc eleifend hendrerit. Cras justo lorem, sodales vel, consequat et, posuere non, sem. Duis cursus massa nec odio. Ut risus. Duis et sem ac lectus interdum ornare. Aenean interdum. Curabitur libero libero, gravida ac, porta eget, tincidunt ut, massa. Curabitur eu nisl. Sed tempus. Duis lectus diam, varius at, vehicula non, sollicitudin ac, ligula. Fusce porttitor dui vel dolor. Nunc euismod rutrum neque. Nulla vel magna quis arcu dictum cursus. Morbi porta turpis in purus. Duis neque odio, consectetuer vel, aliquet quis, suscipit in, arcu. Praesent lectus. Praesent accumsan.

Aliquam et dui a eros bibendum iaculis. Integer quam tellus, pulvinar sit amet, dignissim vitae, mollis ut, est. Aenean volutpat. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Sed sed ipsum dignissim nisl porttitor facilisis. Curabitur iaculis nisl vel magna. Nullam posuere purus non eros. Vestibulum non enim. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Quisque urna. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos hymenaeos. Nunc commodo luctus est.

Integer in sem. Quisque eu justo. Nam vel sapien nec nibh mattis pulvinar. Nulla dignissim lorem quis ante. Fusce tincidunt scelerisque augue. Vivamus elementum nonummy purus. Ut suscipit suscipit elit. Suspendisse non pede. Proin posuere. Vestibulum leo. Etiam tempus lorem. Vivamus id dui. Aenean sollicitudin nunc venenatis sem. Suspendisse massa nisl, mollis et, convallis vitae, sodales quis, odio. Praesent luctus elit. Donec ante. Phasellus tempus. Integer pulvinar ligula sed sapien. Vestibulum ut nisi. Aliquam erat volutpat.
SLAB

longer_slab_of_UTF8_text  = <<SLAB
Lór€m ïpsûm dólór sït àm€t, cóñs€ct€tû€r àdïpïscïñg €lït. S€d mól€stï€ ïpsûm àt sàpï€ñ. ñûllàm t€mpûs, rïsûs €ûïsmód rûtrûm móllïs, p€d€ l€ctûs ûllàmcórp€r ñïsï, €t àûctór àrcû v€lït ût lïgûlà. Dóñ€c àt f€lïs. à€ñ€àñ vûlpûtàt€. S€d s€d dïàm ñóñ m€tûs vàrïûs p€ll€ñt€sqû€. ñûllà àc ñïbh €t m€tûs phàr€trà cóñvàllïs. Phàs€llûs lïb€ró. ñûñc vïv€rrà tïñcïdûñt dólór. Dûïs rïsûs. Próïñ s€mp€r rhóñcûs qûàm. S€d v€stïbûlûm mól€stï€ l€ctûs. Sûsp€ñdïss€ sït àm€t t€llûs àc tórtór tïñcïdûñt cómmódó. Lór€m ïpsûm dólór sït àm€t, cóñs€ct€tû€r àdïpïscïñg €lït. Dûïs v€ñ€ñàtïs ïmp€rdï€t €rós. Dûïs t€mpór €l€m€ñtûm lïb€ró. Mórbï ïd f€lïs. ïñ tïñcïdûñt cóñs€ct€tû€r €ràt. Prà€s€ñt vàrïûs pûlvïñàr l€ctûs.

Cûm sócïïs ñàtóqû€ p€ñàtïbûs €t màgñïs dïs pàrtûrï€ñt móñt€s, ñàsc€tûr rïdïcûlûs mûs. Phàs€llûs cûrsûs tûrpïs ïñ m€tûs. ïñt€g€r trïstïqû€ dïàm à ñûñc. P€ll€ñt€sqû€ hàbïtàñt mórbï trïstïqû€ s€ñ€ctûs €t ñ€tûs €t màl€sûàdà fàm€s àc tûrpïs €g€stàs. Cûràbïtûr s€d sàpï€ñ ïñ l€ctûs àûctór p€ll€ñt€sqû€. ñàm qûïs l€ó ïd làcûs f€rm€ñtûm sàgïttïs. Dûïs s€mp€r ódïó tïñcïdûñt €st. Fûsc€ cóñvàllïs, m€tûs ïd fàûcïbûs pósû€r€, tórtór ûrñà vïv€rrà ñïsl, v€l rhóñcûs mï ûrñà ïñ jûstó. Fûsc€ dïgñïssïm lór€m €g€t àûgû€. Mórbï p€d€ ñïbh, €l€m€ñtûm vïtà€, cóñvàllïs sït àm€t, f€ûgïàt €û, €rós. Dóñ€c blàñdït €st s€d àûgû€. àlïqûàm vûlpûtàt€, l€ó à t€mpór sûscïpït, l€ctûs f€lïs s€mp€r ûrñà, s€d lûctûs p€d€ ñ€qû€ €û tórtór. ñûllà fàcïlïsï.

ût €g€t rïsûs ût €ràt vólûtpàt ûltrïc€s. S€d ñ€c sàpï€ñ ûltrïc€s ñûñc €l€ïf€ñd h€ñdr€rït. Cràs jûstó lór€m, sódàl€s v€l, cóñs€qûàt €t, pósû€r€ ñóñ, s€m. Dûïs cûrsûs màssà ñ€c ódïó. ût rïsûs. Dûïs €t s€m àc l€ctûs ïñt€rdûm órñàr€. à€ñ€àñ ïñt€rdûm. Cûràbïtûr lïb€ró lïb€ró, gràvïdà àc, pórtà €g€t, tïñcïdûñt ût, màssà. Cûràbïtûr €û ñïsl. S€d t€mpûs. Dûïs l€ctûs dïàm, vàrïûs àt, v€hïcûlà ñóñ, sóllïcïtûdïñ àc, lïgûlà. Fûsc€ pórttïtór dûï v€l dólór. ñûñc €ûïsmód rûtrûm ñ€qû€. ñûllà v€l màgñà qûïs àrcû dïctûm cûrsûs. Mórbï pórtà tûrpïs ïñ pûrûs. Dûïs ñ€qû€ ódïó, cóñs€ct€tû€r v€l, àlïqû€t qûïs, sûscïpït ïñ, àrcû. Prà€s€ñt l€ctûs. Prà€s€ñt àccûmsàñ.

àlïqûàm €t dûï à €rós bïb€ñdûm ïàcûlïs. ïñt€g€r qûàm t€llûs, pûlvïñàr sït àm€t, dïgñïssïm vïtà€, móllïs ût, €st. à€ñ€àñ vólûtpàt. V€stïbûlûm àñt€ ïpsûm prïmïs ïñ fàûcïbûs órcï lûctûs €t ûltrïc€s pósû€r€ cûbïlïà Cûrà€; S€d s€d ïpsûm dïgñïssïm ñïsl pórttïtór fàcïlïsïs. Cûràbïtûr ïàcûlïs ñïsl v€l màgñà. ñûllàm pósû€r€ pûrûs ñóñ €rós. V€stïbûlûm ñóñ €ñïm. Cûm sócïïs ñàtóqû€ p€ñàtïbûs €t màgñïs dïs pàrtûrï€ñt móñt€s, ñàsc€tûr rïdïcûlûs mûs. Qûïsqû€ ûrñà. Clàss àpt€ñt tàcïtï sócïósqû àd lïtórà tórqû€ñt p€r cóñûbïà ñóstrà, p€r ïñc€ptós hym€ñà€ós. ñûñc cómmódó lûctûs €st.

ïñt€g€r ïñ s€m. Qûïsqû€ €û jûstó. ñàm v€l sàpï€ñ ñ€c ñïbh màttïs pûlvïñàr. ñûllà dïgñïssïm lór€m qûïs àñt€. Fûsc€ tïñcïdûñt sc€l€rïsqû€ àûgû€. Vïvàmûs €l€m€ñtûm ñóñûmmy pûrûs. ût sûscïpït sûscïpït €lït. Sûsp€ñdïss€ ñóñ p€d€. Próïñ pósû€r€. V€stïbûlûm l€ó. €tïàm t€mpûs lór€m. Vïvàmûs ïd dûï. à€ñ€àñ sóllïcïtûdïñ ñûñc v€ñ€ñàtïs s€m. Sûsp€ñdïss€ màssà ñïsl, móllïs €t, cóñvàllïs vïtà€, sódàl€s qûïs, ódïó. Prà€s€ñt lûctûs €lït. Dóñ€c àñt€. Phàs€llûs t€mpûs. ïñt€g€r pûlvïñàr lïgûlà s€d sàpï€ñ. V€stïbûlûm ût ñïsï. àlïqûàm €ràt vólûtpàt.
SLAB

def sanity_check string
  # should be able to round-trip
  ucs2 = Wikitext::utf8_to_ucs2(string)
  raise if Wikitext::ucs2_to_utf8(ucs2) != string

  # should get the same answer as iconv
  raise if ucs2 != Iconv.iconv('UCS-2LE', 'UTF-8', string).shift
  raise if Wikitext::ucs2_to_utf8(ucs2) != Iconv.iconv('UTF-8', 'UCS-2LE', ucs2).shift
  printf '.'
end

def iconv_to_ucs2 job, description, input
  job.report(description) do
    100_000.times { Iconv.iconv('UCS-2LE', 'UTF-8', input) }
  end
end

def iconv_to_utf8 job, description, input
  job.report(description) do
    100_000.times { Iconv.iconv('UTF-8', 'UCS-2LE', input) }
  end
end

def internal_to_ucs2 job, description, input
  job.report(description) do
    100_000.times { Wikitext::utf8_to_ucs2(input) }
  end
end

def internal_to_utf8 job, description, input
  job.report(description) do
    100_000.times { Wikitext::ucs2_to_utf8(input) }
  end
end

printf "Sanity checking: "
sanity_check short_slab_of_ASCII_text
sanity_check short_slab_of_UTF8_text
sanity_check longer_slab_of_ASCII_text
sanity_check longer_slab_of_UTF8_text
printf "\n"

Benchmark.bmbm do |job|
  iconv_to_ucs2     job, 'iconv    (short ASCII to UCS-2)',       short_slab_of_ASCII_text
  iconv_to_ucs2     job, 'iconv    (short non-ASCII to UCS-2)',   short_slab_of_UTF8_text
  iconv_to_ucs2     job, 'iconv    (longer ASCII to UCS-2)',      longer_slab_of_ASCII_text
  iconv_to_ucs2     job, 'iconv    (longer non-ASCII to UCS-2)',  longer_slab_of_UTF8_text
  internal_to_ucs2  job, 'internal (short ASCII to UCS-2)',       short_slab_of_ASCII_text
  internal_to_ucs2  job, 'internal (short non-ASCII to UCS-2)',   short_slab_of_UTF8_text
  internal_to_ucs2  job, 'internal (longer ASCII to UCS-2)',      longer_slab_of_ASCII_text
  internal_to_ucs2  job, 'internal (longer non-ASCII to UCS-2)',  longer_slab_of_UTF8_text
  iconv_to_utf8     job, 'iconv    (short ASCII to UTF-8)',       Wikitext::utf8_to_ucs2(short_slab_of_ASCII_text)
  iconv_to_utf8     job, 'iconv    (short non-ASCII to UTF-8)',   Wikitext::utf8_to_ucs2(short_slab_of_UTF8_text)
  iconv_to_utf8     job, 'iconv    (longer ASCII to UTF-8)',      Wikitext::utf8_to_ucs2(longer_slab_of_ASCII_text)
  iconv_to_utf8     job, 'iconv    (longer non-ASCII to UTF-8)',  Wikitext::utf8_to_ucs2(longer_slab_of_UTF8_text)
  internal_to_utf8  job, 'internal (short ASCII to UTF-8)',       Wikitext::utf8_to_ucs2(short_slab_of_ASCII_text)
  internal_to_utf8  job, 'internal (short non-ASCII to UTF-8)',   Wikitext::utf8_to_ucs2(short_slab_of_UTF8_text)
  internal_to_utf8  job, 'internal (longer ASCII to UTF-8)',      Wikitext::utf8_to_ucs2(longer_slab_of_ASCII_text)
  internal_to_utf8  job, 'internal (longer non-ASCII to UTF-8)',  Wikitext::utf8_to_ucs2(longer_slab_of_UTF8_text)
end



