! ===========================================================================
!   Inform Language Definition File: Danish 20170510
!   Derived from Norwegian.h dated 20110903
!   Derived from Swedish.h dated 20080404
!   Derived from English.h dated 000629, compatible with English.h dated 040227
!
!   Supplied for use with Inform 6 -- Release 6/11
!
!   Copyright Graham Nelson 1993-2004 but freely usable (see manuals)
!   Translation (c) Thomas Bøvith 2017
!   Based on the Swedish translation (c) Fredrik Ramsberg 2003-2008 and
!   Norwegian translation by Eirik Krogstad 2011 (with permissions).
!
!   This translation has a license which should always be distributed
!   along with it. It basically says that you're free to use the translation,
!   as long as you don't sell it and don't claim that you've written it.
!   You may sell games compiled using this translation though.
!
!   This file is automatically Included in your game file by "parserm".
!   Strictly, "parserm" includes the file named in the "language__" variable,
!   whose contents can be defined by +language_name=XXX compiler setting (with
!   a default of "english").
!
! ---------------------------------------------------------------------------

! Comment conventions:
! *#* means a problem not yet adressed
! ¤#¤ means a problem that's been solved

System_file;

#ifndef LIBRARY_VERSION;
  Message warning "This version of Danish.h was made for use with library 6/11.";
#endif;
#iftrue LIBRARY_VERSION < 611;
  Message warning "This version of Danish.h was made for use with library 6/11.";
#endif;

! ---------------------------------------------------------------------------
!   First we need to replace some routines
! ---------------------------------------------------------------------------

! Routines in Parserm.h, to be replaced by routines in DanRout.h
Replace ResetDescriptors;
Replace Descriptors;
Replace ScoreMatchL;
Replace PrintCommand;
Replace CantSee;
Replace GetGNAOfObject;
Replace PronounNotice;
Replace PrefaceByArticle;

Replace Indefart;
Replace CInDefart;
Replace Defart;
Replace CDefart;

! Routines in Verblibm.h, to be replaced by routines in DanRout.h
Replace WriteAfterEntry;
Replace Objects1Sub;

! ---------------------------------------------------------------------------
!   Part I.   Preliminaries
! ---------------------------------------------------------------------------

Constant LanguageVersion = "Dansk Inform v0.1 oversat af Thomas Bøvith 2017,
    baseret på den norske oversættelse af Eirik Krogstad 2011, baseret på den
    svenske oversættelse af Fredrik Ramsberg 2003-2008.";

Global caps_mode;
Global transcribe_mode;

Attribute def_article;
Attribute uter;

Property short_name_def;

[check_caps obj   c k;
  c = k;
  if(metaclass(obj)==String)
    print (string) obj;
  else
    print (object) obj;
  rtrue;
];

[PrintBuffer buffer_name   i;
  for (i=0 : i<buffer_name-->0 : i++)
    print (char) buffer_name->(i+WORDSIZE);
];


#ifdef DEBUG;

Constant _MAXPARSE 15;
Array _NameTestBuffer -> (160 + 2);

#ifdef TARGET_ZCODE;

Array _BadWords -> _MAXPARSE;

#ifnot; !Glulx

Array _BadWords -> MAX_BUFFER_WORDS;

#endif;


[_CheckName o prop   i badcount;
  caps_mode=false;
  indef_mode=false;
  PrintToBuffer(_NameTestBuffer, 160, o, prop);
#ifdef TARGET_ZCODE;
  parse->0 = _MAXPARSE;
  for(i=2:i < (2 + _NameTestBuffer -> 1): i++) {
    _NameTestBuffer -> i = LowerCase(_NameTestBuffer -> i);
    if((_NameTestBuffer -> i)==','  or '.'  or '"')
    _NameTestBuffer -> i = ' ';
  }
#ifnot; !Glulx
  for(i=4:i < (4 + _NameTestBuffer --> 0): i++) {
    _NameTestBuffer -> i = LowerCase(_NameTestBuffer -> i);
    if((_NameTestBuffer -> i)==','  or '.'  or '"')
    _NameTestBuffer -> i = ' ';
  }
#endif;
  Tokenise__(_NameTestBuffer, parse);
  if(o.parse_name~=0) { ! Method 1: Check with object parse_name-routine
    parser_action = NULL;
    wn=1;
    i = RunRoutines(o, parse_name);
#ifdef TARGET_ZCODE;
    if(i < parse->1)
      _BadWords->(badcount++) = i;
  } else {! Method 2: Check the object name-property
    for(i=0:i < parse->1 && i < _MAXPARSE: i++)
      if(~~WordInProperty(parse-->(1+i*2), o, name))
        _BadWords->(badcount++) = i;
  }
#ifnot; !Glulx
    if(i < parse-->0)
      _BadWords->(badcount++) = i;
  } else {! Method 2: Check the object name-property
    for(i=0:i < parse-->0 && i < MAX_BUFFER_WORDS: i++)
      if(~~WordInProperty(parse-->(1+i*3), o, name))
        _BadWords->(badcount++) = i;
  }
#endif;
  return badcount;
];

[_PrintBadWords count  i j k;
  print "(";
  for(k=0: k<count: k++) {
    i = _BadWords -> k;
#ifdef TARGET_ZCODE;
    for(j=0: j < parse -> (4+i*4): j++) {
      print (char) _NameTestBuffer -> (j + parse->(5+i*4));
    }
#ifnot; !Glulx
    for(j=0: j < parse --> (2+i*3): j++) {
      print (char) _NameTestBuffer -> (j + parse-->(3+i*3));
    }
#endif;
    if(k < count - 1)
      print ", ";
  }
  print ")";
];

[_CheckThisObj obj   j isNormalObj;
  isNormalObj=parent(obj)~=0 || obj hasnt light;
  if(isNormalObj && parent(obj)==0) { ! Check if dark, lonely objects have exits
    objectloop(j in compass) isNormalObj=isNormalObj && (obj.(j.door_dir)==0);
  }
  return isNormalObj;
];

[CheckGameSub  i err anyerr wordno specific;
  specific=noun;
#ifdef TARGET_GLULX;
  if(specific==10000 && TryNumber(2)==10000) {
    ! Parse object number manually
    specific=0;
    for(i=0:i<WordLength(2):i++) {
      specific=10 * specific + ((WordAddress(2)->i) - '0');
    }
  }
#endif;
  style bold;
  if(specific) print "Undersøger blot: ", (name) specific, "^";
  print  "Ser efter følgende indikatorer på problemer:
    ^* Mangler både short_name_def og short_name.
    ^* Har short_name OG short_name_indef eller short_name_def.
    ^* Har ord i nogle af navnestrengene/-rutinene som mangler som synonym til objektet.
    ^^";
  style roman;
  anyerr=false;
  objectloop(i>LibraryMessages && ~~(i ofclass class)) {
    err=false;
!    isNormalObj=parent(i)~=0 || i hasnt light;
!    if(isNormalObj && parent(i)==0) { ! Check if dark, lonely objects have exits
!      objectloop(j in compass) isnormalobj=isnormalobj && (i.(j.door_dir)==0);
!    }
    if((specific==0 && _CheckThisObj(i)) || specific==i) {
      if(i.short_name_def==0 && i.short_name==0 && i hasnt proper) {
        err=true;
        print "Mangler både short_name_def og short_name: ", (name) i, ".^";
      } else if(i.short_name~=0 && (i.short_name_def~=0 || i.short_name_indef~=0)) {
        err=true;
        print "Har short_name OG short_name_indef eller short_name_def: ", (name) i, ".^";
      }
      if(i.short_name~=0) {
        wordno=_CheckName(i, short_name);
        if(wordno) {
          print "Har ord i short_name som mangler som synonym: ", (name) i, " ",(_PrintBadWords) (wordno),".^";
          err=true;
        }
      }
      if(i.short_name_def~=0) {
        wordno=_CheckName(i, short_name_def);
        if(wordno) {
          print "Har ord i short_name_def som mangler som synonym: ", (name) i, " ",(_PrintBadWords) (wordno),".^";
          err=true;
        }
      }
      if(i.short_name_indef~=0) {
        wordno=_CheckName(i, short_name_indef);
        if(wordno) {
          print "Har ord i short_name_indef som mangler som synonym: ", (name) i, " ",(_PrintBadWords) (wordno),".^";
          err=true;
        }
      }
      if(i.short_name==0 && i.short_name_indef==0) {
        wordno=_CheckName(i);
        if(wordno) {
          print "Har ord i objektets navnestreng som mangler som synonym: ", (name) i, " ",(_PrintBadWords) (wordno),".^";
          err=true;
        }
      }
    }

    if(err) {
      anyerr=true;
    }

  }
  if(~~anyerr) "Ingen problemer fundet.";
];

[CheckGameSelectSub  i;
  style bold;
  print  "Objekter med stjerner rundt navnet undersøges IKKE af falkeblik:^";
  style roman;
  objectloop(i>LibraryMessages && ~~(i ofclass class)) {
    if(_CheckThisObj(i)) {
      print "(",i,")  ", (name) i, "^";
    } else {
      print "(",i,") *** ", (name) i, " ***^";
    }
  }
];

#endif;

[SetGender x gender;
  if(gender == 0) {
    if(x has male or female or uter or neuter) rtrue;
    if(x has animate) gender = LanguageAnimateGender;
    else gender = LanguageInanimateGender;
  } else {
    give x ~male;
    give x ~female;
    give x ~neuter;
    give x ~uter;
  }
  switch(gender) {
    male: give x male;
    female: give x female;
    uter: give x uter;
    neuter: give x neuter;
  }
];

Class   CompassDirection
  with  number 0,
        description [;
            if (location provides compass_look && location.compass_look(self)) rtrue;
            if (self.compass_look()) rtrue;
            L__M(##Look, 7, self);
        ],
        compass_look false,
  has   scenery proper;
Object Compass "compass" has concealed;
IFNDEF WITHOUT_DIRECTIONS;
CompassDirection -> n_obj "nord"
                    with door_dir n_to, name 'nord' 'n//' 'nordpå' 'nordover';
CompassDirection -> s_obj "syd"
                    with door_dir s_to, name 'syd' 's//' 'sødpå' 'sydover';
CompassDirection -> e_obj "øst"
                    with door_dir e_to, name 'øst' 'ø//' 'e//' 'østpå' 'østover';
CompassDirection -> w_obj "vest"
                    with door_dir w_to, name 'vest' 'v//' 'w//' 'vestpå' 'vestover';
CompassDirection -> ne_obj "nordøst"
                    with door_dir ne_to, name 'nordøst' 'nø' 'nordøstpå' 'nordøstover';
CompassDirection -> nw_obj "nordvest"
                    with door_dir nw_to, name 'nordvest' 'nv' 'nordvestpå' 'nordvestover';
CompassDirection -> se_obj "sydøst"
                    with door_dir se_to, name 'sydøst' 'sø' 'sydøstpå' 'sydøstover';
CompassDirection -> sw_obj "sydvest"
                    with door_dir sw_to, name 'sydvest' 'sv' 'sydvestpå' 'sydvestover';
CompassDirection -> u_obj "op"
                    with door_dir u_to, name 'op' 'o//' 'opad' 'taget' 'himlen';
CompassDirection -> d_obj "ned"
                    with door_dir d_to, name 'ned' 'nedad' 'gulv' 'gulvet' 'jord' 'jorden';
ENDIF;
CompassDirection -> in_obj "ind"
                    with door_dir in_to, name 'ind' 'indenfor' 'indad';
CompassDirection -> out_obj "ud"
                    with door_dir out_to, name 'ud' 'udenfor' 'udad';

! ---------------------------------------------------------------------------
!   Part II.   Vocabulary
! ---------------------------------------------------------------------------

! Ændringer for CustomTense:
! Nu er følgende variabler globale i stedet for konstanter:
! De gamle konstantene er udkommenterede for at vise hvor de tidligere var.

Global IS__TX        = " er";
Global ARE__TX       = " er";
Global IS2__TX       = "er ";
Global ARE2__TX      = "er ";

Global YOU__TX       = "Du";
Global LYOU__TX      = "du";
Global FORMER__TX    = "dit tidligere jeg";
Global YOURSELF__TX  = "dig selv";
Constant MYSELF__TX  = "mig selv";

! Slut med ændringer for CustomTense:

Constant AGAIN1__WD   = 'igen';
Constant AGAIN2__WD   = 'g//';
Constant AGAIN3__WD   = 'again';
Constant OOPS1__WD    = 'ups';
Constant OOPS2__WD    = 'oops';
Constant OOPS3__WD    = 'ups';
Constant UNDO1__WD    = 'fortryd';
Constant UNDO2__WD    = 'undo';
Constant UNDO3__WD    = 'fortryd';

Constant ALL1__WD     = 'alle';
Constant ALL2__WD     = 'hver';
Constant ALL3__WD     = 'allesammen';
Constant ALL4__WD     = 'samtlige';
Constant ALL5__WD     = 'alt';
Constant AND1__WD     = 'og';
Constant AND2__WD     = 'samt';
Constant AND3__WD     = 'og';
Constant BUT1__WD     = 'undtagen';
Constant BUT2__WD     = 'undtagen';
Constant BUT3__WD     = 'undtagen';
Constant ME1__WD      = 'jeg';
Constant ME2__WD      = 'mig';
Constant ME3__WD      = 'selv';
Constant OF1__WD      = 'af';
Constant OF2__WD      = 'blandt';
Constant OF3__WD      = 'af';
Constant OF4__WD      = 'af';
Constant OTHER1__WD   = 'andre';
Constant OTHER2__WD   = 'anden';
Constant OTHER3__WD   = 'andet';
Constant THEN1__WD    = 'derefter';
Constant THEN2__WD    = 'herefter';
Constant THEN3__WD    = 'derefter';

Constant NO1__WD      = 'n//';
Constant NO2__WD      = 'nej';
Constant NO3__WD      = 'næ';
Constant YES1__WD     = 'j//';
Constant YES2__WD     = 'ja';
Constant YES3__WD     = 'jo';

Constant AMUSING__WD  = 'amusing';
Constant FULLSCORE1__WD = 'fullscore';
Constant FULLSCORE2__WD = 'full';
Constant QUIT1__WD    = 'afslut';
Constant QUIT2__WD    = 'quit';
Constant RESTART__WD  = 'restart';
Constant RESTORE__WD  = 'restore';

! ¤#¤ The need for uter, in addition to male, female and neuter, necessitates
! the adding of four bits to every GNA-word. While there was indeed room for
! these bits, it still implies that one or more of the other library files will
! have to be modified as well to interpret these words correctly in order to
! handle the four genders correctly.

Array LanguagePronouns table ! ¤#¤ Usage of this array must be modified to add uter

   !  word       possible GNAs                   connected
   !             to follow:                      to:
   !             a       i
   !             s   p   s   p
   !             mfunmfunmfunmfun

      'han'    $$1000000010000000                NULL
      'hende'  $$0100000001000000                NULL
      'den'    $$0010000000100000                NULL
      'det'    $$0001000000010000                NULL
      'dem'    $$0000111100001111                NULL;
!      'de'    $$0000111100001111                NULL; !¤#¤ Decided to leave this out because it gets ugly when player types "pronomen" (pronouns)

! 'de' når pronomenet er subjekt
! 'dem' ved direkte eller indirekte objekt, styring til preposition

Array LanguageDescriptors table ! ¤#¤ Usage of this array must be modified to add uter

   !  word       possible GNAs    descriptor     connected
   !             to follow:       type:          to:
   !             a       i
   !             s   p   s   p
   !             mfunmfunmfunmfun

      'min'    $$1110000011100000 POSSESS_PK      0
      'mit'    $$0001000000010000 POSSESS_PK      0
      'mine'   $$0000111100001111 POSSESS_PK      0
! *#* Hard to translate and not very important, skipping
!      'this'   $$111111111111    POSSESS_PK      0
!      'these'  $$000111000111    POSSESS_PK      0
!      'that'   $$111111111111    POSSESS_PK      1
!      'those'  $$000111000111    POSSESS_PK      1
      'hans'   $$1111111111111111 POSSESS_PK      'han'
      'hendes' $$1111111111111111 POSSESS_PK      'hende'
      'deres'  $$1111111111111111 POSSESS_PK      'dem'
      'dens'   $$1111111111111111 POSSESS_PK      'den'
      'dets'   $$1111111111111111 POSSESS_PK      'det'
      'den'    $$1110000011100000 DEFART_PK       NULL ! ¤#¤ These have been commented out because their usage
      'det'    $$0001000000010000 DEFART_PK       NULL !     as definite articles conflict with their usage as
      'de'     $$0000111100001111 DEFART_PK       NULL !     it-words. Should be corrected in the parser somehow.
      'en'     $1110000011100000 INDEFART_PK      NULL
      'et'     $$0001000000010000 INDEFART_PK     NULL
      'nogen'  $$0000111100001111 INDEFART_PK     NULL
      'tænd'   $$1110000011100000 light           NULL
      'tændt'  $$0001000000010000 light           NULL
      'tænde'  $$1111111111111111 light           NULL
      'tændte' $$1000000010000000 light           NULL
     'slukket' $$1111000011110000 (-light)        NULL
    'slukkede' $$1000000010000000 (-light)        NULL;

Array LanguageNumbers table
    'en' 1 'et' 1 'to' 2 'par' 2 'tre' 3 'fire' 4 'fem' 5
    'seks' 6 'syv' 7 'otte' 8 'ni' 9 'ti' 10
    'elleve' 11 'tolv' 12 'tretten' 13 'fjorten' 14 'femten' 15
    'seksten' 16 'sytten' 17 'atten' 18 'nitten' 19 'tyve' 20;

! ---------------------------------------------------------------------------
!   Part III.   Translation
! ---------------------------------------------------------------------------

[LTI_delete pos n   k m;
!  print pos,",", n, ".";
  if(n<1) n=1;
#ifdef TARGET_ZCODE;
  m=(buffer->1)+3-n;
#ifnot; ! TARGET_GLULX
  m=(buffer-->0)+5-n;
#endif; ! TARGET_
  for(k=pos:k<m:k++)
    buffer->k=buffer->(k+n);
#ifdef TARGET_ZCODE;
  buffer->1=(buffer->1)-n;
#ifnot; ! TARGET_GLULX
  buffer-->0=(buffer-->0)-n;
#endif; ! TARGET_
];

! *#* One could definitely do a lot more work here...

[ LanguageToInformese   i c1 c2 cnew no_transcribe changed printed_first charcount;

! This section handles transciption of aa, ae and oe into å, æ and ø respectively

#ifdef TARGET_ZCODE;
  if(transcribe_mode && buffer->1>1) {
    for(i=2:i<1+buffer->1:i++) { ! Stop one character before the last
#ifnot; ! TARGET_GLULX
  if(transcribe_mode && buffer-->0>1) {
    for(i=4:i<3+buffer-->0:i++) { ! Stop one character before the last
#endif; ! TARGET_
!      print "#";
      c1=buffer->i;
      c2=buffer->(i+1);
      if(c1=='.')
        break;
      if(c1=='!') {
        no_transcribe++;
        LTI_delete(i);
        i--;
        changed=1;
        continue;
      }

#ifdef TARGET_ZCODE;
      if(c1=='a') {
        if(c2=='a')
          cnew='å';
        else if(c2=='e')
          cnew='æ';
      } else if(c1=='o' && c2=='e')
        cnew='ø';
#ifnot; ! TARGET_GLULX
      if(c1=='a') {
        if(c2=='a' or 'A')
          cnew='å';
        else if(c2=='e' or 'E')
          cnew='æ';
      } else if(c1=='o' && c2=='e' or 'E')
        cnew='ø';
      else if(c1=='A') {
        if(c2=='a' or 'A')
          cnew='Å';
        else if(c2=='e' or 'E')
          cnew='Æ';
      } else if(c1=='O' && c2=='e' or 'E')
        cnew='Ø';
#endif; ! TARGET_

      if(cnew) {
        if(no_transcribe)
          no_transcribe--;
        else {
          buffer->i=cnew;
          LTI_delete(i+1);
          changed=1;
        }
        cnew=0;
      }
    }
    if(changed)
      Tokenise__(buffer,parse);

    if(transcribe_mode>1 && changed) {
      print "(blev transkriberet til ~";
      printed_first = false;
#ifdef TARGET_ZCODE;
      charcount = 2+buffer->1;
#ifnot; ! TARGET_GLULX
      charcount = 4+buffer-->0;
#endif; ! TARGET_
      for(i=WORDSIZE:i<charcount:i++) {
        if(printed_first || buffer->i~=' ') {
          printed_first=true;
          if(buffer->i=='.')
            break;
          print (char) buffer->i;
        }
      }

      print "~)";
      new_line;
    }

  }

! This section changes "mig selv" to "mig", to avoid a pronoun consisting of two words

#ifdef TARGET_ZCODE;
  for(i=2:i<=parse->1:i++)
    if(parse-->(2*i-1)=='selv' && parse-->(2*i-3)=='mig') {
      LTI_delete((parse->(4*i+1))-1,(parse->(4*i))+1);
      Tokenise__(buffer,parse);
    }
#ifnot; ! TARGET_GLULX
  for(i=2:i<=parse-->0:i++)
    if(parse-->(3*i-2)=='selv' && parse-->(3*i-5)=='mig') {
      LTI_delete((parse-->(3*i))-1,(parse-->(3*i-1))+1);
      Tokenise__(buffer,parse);
    }
#endif; ! TARGET_

];

[ TranscribeOnSub;
  if(transcribe_mode==0 or 2)
    L__M(##TranscribeOn,2);
  else
    L__M(##TranscribeOn,1);
  transcribe_mode=1;
];

[ TranscribeShowSub;
  if(transcribe_mode==0 or 1)
    L__M(##TranscribeShow,2);
  else
    L__M(##TranscribeShow,1);
  transcribe_mode=2;
];

[ TranscribeOffSub;
  if(transcribe_mode>0)
    L__M(##TranscribeOff,2);
  else
    L__M(##TranscribeOff,1);
  transcribe_mode=0;
];

! ---------------------------------------------------------------------------
!   Part IV.   Printing
! ---------------------------------------------------------------------------

Constant LanguageAnimateGender   = male;
Constant LanguageInanimateGender = uter;

Constant LanguageContractionForms = 1;     ! Danish has only 1:
                                           ! 0 = no matter what

[ LanguageContraction text x;
  x=text; ! ¤#¤ Only here to avoid a warning
  return 0;
];

Array LanguageArticles -->

 !   Contraction form 0:
 !   Cdef   Def    Indef

! ¤#¤ Cdef and Def should not be printed unless there is an adjective before the noun

     "Den " "den " "en "        ! masc/fem/uter sing
     "Det " "det " "et "        ! neuter sing
     "De "  "de "  "nogle ";    ! plural

                   !             a               i
                   !             s       p       s       p
                   !             m f u n m f u n m f u n m f u n

Array LanguageGNAsToArticles --> 0 0 0 1 2 2 2 2 0 0 0 1 2 2 2 2; ! ¤#¤ Usage of this array must be modified to add uter

[LanguagePrintShortName o  i;
  if(indef_mode==false)
    i=short_name_def;
  else
    i=short_name_indef;
   if(o.&i~=0) {
     if(metaclass(o.i)==String) {
       print (string) o.i;
       rtrue;
     }
     else if(metaclass(o.i)==Routine) {
       if(PrintOrRun(o, i)==true)
         rtrue;
     } else
       rtrue; ! Incorrect type of value
   }
   if(o.&short_name~=0) {
     if(metaclass(o.short_name)==String) {
       print (string) o.short_name;
       rtrue;
     }
     else if(metaclass(o.short_name)==Routine) {
       if(PrintOrRun(o, short_name)==true)
         rtrue;
     } else
       rtrue; ! Incorrect type of value
   }
  print (object) o;
  rtrue;
];

[ LanguageDirection d;
  switch(d)
  {
     n_to: print "nord";
     s_to: print "syd";
     e_to: print "øst";
     w_to: print "vest";
     ne_to: print "nordøst";
     nw_to: print "nordvest";
     se_to: print "sydøst";
     sw_to: print "sydvest";
     u_to: print "op";
     d_to: print "ned";
     in_to: print "ind";
     out_to: print "ud";
     default: return RunTimeError(9,d);
  }
];

Constant ENDS_WITH_T = 2;

[ LanguageNumber n force_neuter_1 obj gender f;
  if(obj~=0 && metaclass(obj)==Object) {
    if(obj has neuter) gender=neuter;
    else gender=uter;
  }
  if((gender==neuter || force_neuter_1~=0) && n==1) { print "et"; return ENDS_WITH_T;}
  if (n==0)    { print "nul"; rfalse; }
  if (n<0)     { print "minus "; n=-n; }
  if (n>=1000) {
    if(LanguageNumber(n/1000, force_neuter_1, false, neuter)~=ENDS_WITH_T)
      print "t";
    print "usind";
    n=n%1000;
    f=1;
  }
  if (n>=100)  { LanguageNumber(n/100, force_neuter_1, false, neuter); print "hundrede"; n=n%100; f=1; }
  if (n==0) rfalse;
  switch(n) ! Fix this to deal with Danish numerals
  {   1:  if(force_neuter_1) { print "et"; return ENDS_WITH_T; } else print "en";
      2:  print "to";
      3:  print "tre";
      4:  print "fire";
      5:  print "fem";
      6:  print "seks";
      7:  print "syv";
      8:  print "otte";
      9:  print "ni";
      10: print "ti";
      11: print "elleve";
      12: print "tolv";
      13: print "tretten";
      14: print "fjorten";
      15: print "femten";
      16: print "seksten";
      17: print "sytten";
      18: print "atten";
      19: print "nitten";
      20 to 99:
          switch(n/10)
          {  2: print "tyve";
             3: print "tredive";
             4: print "fyrre";
             5: print "halvtres";
             6: print "treds";
             7: print "halvfjerds";
             8: print "firs";
             9: print "halvfems";
          }
          if (n%10 ~= 0) return LanguageNumber(n%10, force_neuter_1);
  }
];

Constant DanishNumber LanguageNumber;

[ LanguageTimeOfDay hours mins;
   print hours/10, hours%10, ":", mins/10, mins%10;
];

[ LanguageVerb i;
    switch (i) {
      'i//', 't//':     print "status";
      'k//', 'l//':     print "se";
      'u//', 'x//':     print "undersøg";
      'z//':            print "vent";
      'indspilning':    print "indspilning";
      'meddelelser':    print "meddelelser";
      'substantiv':     print "substantiv";
      'transkriber':    print "transkriber";
      'transkription':  print "transkription";
      'afspilning':     print "afspilning";
      default: rfalse;
    }
    rtrue;
];

! ----------------------------------------------------------------------------
!  LanguageVerbIsDebugging is called by SearchScope.  It should return true
!  if word w is a debugging verb which needs all objects to be in scope.
! ----------------------------------------------------------------------------

#Ifdef DEBUG;
[ LanguageVerbIsDebugging w;
    if (w == 'purloin' or 'tree' or 'abstract'
                       or 'gonear' or 'scope' or 'showobj')
        rtrue;
    rfalse;
];
#Endif;

! ----------------------------------------------------------------------------
!  LanguageVerbLikesAdverb is called by PrintCommand when printing an UPTO_PE
!  error or an inference message.  Words which are intransitive verbs, i.e.,
!  which require a direction name as an adverb ('walk west'), not a noun
!  ('I only understood you as far as wanting to touch /the/ ground'), should
!  cause the routine to return true.
! ----------------------------------------------------------------------------

[ LanguageVerbLikesAdverb w;
    if (w == 'se' or 'kig' or 'gå' or 'løb' or 'skub' or 'tryk')
        rtrue;
    rfalse;
];

! ----------------------------------------------------------------------------
!  LanguageVerbMayBeName is called by NounDomain when dealing with the
!  player's reply to a "Which do you mean, the short stick or the long
!  stick?" prompt from the parser. If the reply is another verb (for example,
!  LOOK) then then previous ambiguous command is discarded /unless/
!  it is one of these words which could be both a verb /and/ an
!  adjective in a 'name' property.
! ----------------------------------------------------------------------------

[ LanguageVerbMayBeName w;
    if (w == 'full' or 'fuld' or 'lang' or 'lange' or 'normal' or 'normale' or 'kort')
        rtrue;
    rfalse;
];

Constant NKEY__TX     = "N = næste emne";
Constant PKEY__TX     = "F = forrige emne";
Constant QKEY1__TX    = "X = tilbage til spillet";
Constant QKEY2__TX    = "X = forrige menu";
Constant RKEY__TX     = "ENTER = læs emne";

Constant NKEY1__KY    = 'N';
Constant NKEY2__KY    = 'n';
Constant PKEY1__KY    = 'F';
Constant PKEY2__KY    = 'f';
Constant QKEY1__KY    = 'X';
Constant QKEY2__KY    = 'x';

Constant SCORE__TX    = "Point: ";
Constant MOVES__TX    = "Træk: ";
Constant TIME__TX     = "Tid: ";
Constant CANTGO__TX   = "Du kan ikke gå den vej.";
!Constant FORMER__TX   = "ditt tidligere jeg";
!Constant YOURSELF__TX = "dig selv";
!Constant YOU__TX      = "Du";
Constant DARKNESS__TX = "Mørke";

Constant THOSET__TX   = "dem";
Constant THAT__TX     = "den"; ! ¤#¤ Should this be "det" in case we're talking about something neuter?
Constant OR__TX       = " eller ";
Constant NOTHING__TX  = "ingenting";
!Constant IS__TX       = " er";
!Constant ARE__TX      = " er";
!Constant IS2__TX      = "er ";
!Constant ARE2__TX     = "er ";
Constant AND__TX      = " og ";
Constant WHOM__TX     = "som ";
Constant WHICH__TX    = "som ";
Constant COMMA__TX    = ", ";

[ GetSecondaryGNAOfObject obj case gender;
   if (obj hasnt animate) case = 8;

   if (obj has uter) gender = uter;
   if (obj has neuter) gender = neuter;
   if (obj has female) gender = female;
   if (obj has male) gender = male;

   if (gender == 0)
   {   if (case == 0) gender = LanguageAnimateGender;
       else gender = LanguageInanimateGender;
   }
   if (gender == female) case = case + 1;

   if (gender == uter) case = case + 2;
   if (gender == neuter) case = case + 3;
   if (obj has pluralname) case = case + 4;
   return case;
];

[ DenEllerHan obj;   ! Used in the nominative
  SetGender(obj);
  if (obj == player) { print (string) LYOU__TX; return; }
  if (obj has pluralname) { print "de"; return; }
  if (obj has uter) { print "den"; return; }
  if (obj has neuter) { print "det"; return; }
  if (obj has female) { print "hun"; return; }
  print "han";
];
[ CDenEllerHan obj;   ! Used in the accusative
  SetGender(obj);
  if (obj == player) { print (string) YOU__TX; return; }
  if (obj has pluralname) { print "De"; return; }
  if (obj has uter) { print "Den"; return; }
  if (obj has neuter) { print "Det"; return; }
  if (obj has female) { print "Hun"; return; }
  print "Han";
];
[ DenEllerHam obj;   ! Used in the accusative
  SetGender(obj);
  if (obj == player) { print (string) YOURSELF__TX; return; }
  if (obj has pluralname) { print "dem"; return; }
  if (obj has uter) { print "den"; return; }
  if (obj has neuter) { print "det"; return; }
  if (obj has female) { print "hende"; return; }
  print "ham";
];
[ CDenEllerHam obj;   ! Used in the accusative
  SetGender(obj);
  if (obj == player) {
    PrintToBuffer(StorageForShortName, 160, YOURSELF__TX);
    StorageForShortName->WORDSIZE =
        UpperCase(StorageForShortName->WORDSIZE);
    PrintBuffer(StorageForShortName);
    return;
  }
  if (obj has pluralname) { print "Dem"; return; }
  if (obj has uter) { print "Den"; return; }
  if (obj has neuter) { print "Det"; return; }
  if (obj has female) { print "Hende"; return; }
  print "Ham";
];

[ IndefAdjOpen obj;
  if (obj hasnt pluralname) {
    if (obj has neuter) { print "åbent"; return; }
    print "åben"; return;
  }
  print "åbne";
];
[ IndefAdjClosed obj;
  if (obj hasnt pluralname) {
    if (obj has neuter) { print "lukket"; return; }
    print "lukket"; return;
  }
  print "lukkede";
];
[ IndefAdjEmpty obj;
  if (obj hasnt pluralname) {
    if (obj has neuter) { print "tomt"; return; }
    print "tom"; return;
  }
  print "tomme";
];
[ IndefAdjWorn obj;
  if (obj hasnt pluralname) {
    print "båret"; return;
  }
  print "bårne";
];
[ IndefAdjLocked obj;
  if (obj hasnt pluralname) { print "låst"; return; }
  print "låste";
];
[ IndefAdjHeld obj;
  if (obj hasnt pluralname) {
    print "båret"; return;
  }
  print "bårne";
];
[ IndefAdjGiven obj;
  if (obj hasnt pluralname) {
    if (obj has neuter) { print "bortgivet"; return; }
    print "bortgiven"; return;
  }
  print "bortgivne";
];

[ WhichorWhom obj;
  if (obj has pluralname) {print "hvilke"; return; }
  if (obj has neuter) {print "hvilket"; return; }
  print "hvilken";
];

[ LanguageLM n x1;

  Prompt:  print "^>";

  Answer, Ask:    "Du får ikke noget svar.";
  Attack:         "Vold er nok ikke løsningen her.";
  Blow:           "Det virker meningsløst at blæse på ", (DenEllerHam) x1, ".";
  Burn:           "Det virker meget lidt produktivt og desuden farligt.";
  Buy:            "Det er ikke noget man kan købe.";
  Climb:          "Det virker ikke særlig konstruktivt.";
  Close:   switch(n)
           {   1: print_ret (CDenEllerHan) x1," er ikke noget du kan lukke.";
               2: print_ret (CDenEllerHan) x1,
                  " er allerede ",(IndefAdjClosed) x1,".";
               3: "Du lukker ", (the) x1, ".";
           }
  CommandsOff: switch (n) {
        1: "[Kommandoindspilning (SCRIPT) afsluttet.]";
        #Ifdef TARGET_GLULX;
        2: "[Kommandoindspilning (SCRIPT) var allerede slukket.]";
        #Endif; ! TARGET_
    }
  CommandsOn: switch (n) {
        1: "[Kommandoindspilning (SCRIPT) startet.]";
        #Ifdef TARGET_GLULX;
        2: "[Kommandoer afspilles (REPLAY) lige nu.]";
        3: "[Kommandoindspilning (SCRIPT) var allerede tændt.]";
        4: "[Kommandoindspilning (SCRIPT) mislykkedes.]";
        #Endif; ! TARGET_
    }
  CommandsRead: switch (n) {
        1: "[Afspiller (REPLAY) kommandoer.]";
        #Ifdef TARGET_GLULX;
        2: "[Kommanderne afspilles (REPLAY) allerede.]";
        3: "[Afspilning (REPLAY) af kommandoer mislykkedes. Kommandoindspilning (SCRIPT) er tændt.]";
        4: "[Afspilning (REPLAY) af kommandoer mislykkedes.]";
        5: "[Afspilning (REPLAY) af kommandoer er nu fuldført.]";
        #Endif; ! TARGET_
    }
  Consult:        "Du finder intet af interesse i ", (the) x1, ".";
  Cut:            "At skære i ", (DenEllerHam) x1, " virker ikke særlig konstruktivt.";
  Dig:            "Det har ikke noget formål at grave her.";
  Disrobe: switch(n)
           {   1: "Du har ikke ", (CDenEllerHam) x1, " på.";
               2: "Du tager ", (the) x1, " af.";
           }
  Drink:          "Der findes ingenting som er værd at drikke her.";
  Drop:    switch(n)
           {   1: print_ret (CDenEllerHan) x1, " er her allerede.";
               2: "Du har ikke ", (DenEllerHam) x1, ".";
               3: "(tager først ", (the) x1, " af)";
               4:  "Du smider ", (the) x1, ".";
           }
  Eat:     switch(n)
           {   1: print (CDenEllerHan) x1," er uspiselig";
                  if(x1 has pluralname) "e.";
                  if(x1 has neuter) "t."; else ".";
               2: "Du spiser ", (the) x1, ". Ikke værst.";
           }
  EmptyT:  switch(n)
           {   1: print_ret (The) x1, " kan ikke indeholde noget.";
               2: print_ret (The) x1, " er ",(IndefAdjClosed) x1,".";
               3: print_ret (The) x1, " er allerede ",(IndefAdjEmpty) x1,".";
               4: "Det er formålsløst.";
           }
  Enter:   switch(n)
           {   1: print "Men du er jo allerede ";
                  if (x1 has supporter) print "på "; else print "i ";
                  print_ret (the) x1, ".";
               2: print (CDenEllerHan) x1, " er ikke egnet til "; ! ¤#¤ Should be more varied.
                  switch (verb_word) {
                      'stå', 'stil': "at stå på.";
                      'sid', 'sæt': "at sidde på.";
                      'lig', 'læg': "at ligge på.";
                      default: "det.";
                  }
               3: print_ret (The) x1, " er ",(IndefAdjClosed) x1,", så det bliver vanskeligt.";
               4: "Du må stå på egne ben først.";
               5: switch (verb_word) {
                      'stå','stil': print "Du rejser dig";
                      'sid','sæt': print "Du sætter dig";
                      'lig','læg': print "Du lægger dig";
                      default: print "Du springer "; if(x1 has supporter) print "op"; else print "ind";
                  }
                  if (x1 has supporter) print " på "; else print " i ";
                  print_ret (the) x1, ".";
               6: "(du forlader ",(the) x1, ")";
               7: if (x1 has supporter) "(spinger op på ", (the) x1, ")^";
                  if (x1 has container) "(springer ind i ", (the) x1, ")^";
                  "(går på ", (the) x1, ")^";
           }
  Examine: switch(n)
           {   1: "Mørke, substantiv. Fuldstændigt eller næsten fuldstændigt fravær af dagslys eller lys fra lamper, lygter m.m.";
               2: "Du ser ikke noget specielt ved ", (the) x1, ".";
               3: print (The) x1, " er ";
                  if (x1 has on) "tændt."; else "slukket.";
           }
  Exit:    switch(n)
           {   1: "Men du er ikke inde i noget lige nu.";
               2: "Det bliver vanskeligt, siden ", (the) x1, " er ",(IndefAdjClosed) x1,".";
               3: "Du forlader ",(the) x1, ".";
               4: print "Men du er ikke ";
                  if (x1 has supporter) print "på "; else print "i ";
                  print_ret (the) x1, ".";
           }
  Fill:           "Men der findes ikke noget at fylde op med her.";
  FullScore: switch(n)
           {   1: if (deadflag) print "Du fik ";
                  else          print "Du har ";
                  "point, fordelt på:^";
               2: "finde viktige ting";
               3: "besøge vigtige steder";
               4: print "totalt (af ", MAX_SCORE; ")";
           }
  GetOff:         "Men du er ikke på ", (the) x1, " i øjeblikket.";
  Give:    switch(n)
           {   1: "Du holder ikke ", (the) x1, ".";
               2: "Du holder ", (the) x1,
                  " ud foran dig, men der sker ikke det store.";
               3: print_ret (The) x1, " lader ikke til at bryde sig om tilbuddet.";
           }
  Go:      switch(n)
           {   1: print "Først må du klatre ";
                  if (x1 has supporter) print "ned fra "; else print "ud af ";
                  print_ret (the) x1, ".";
               2: print_ret (string) CANTGO__TX;   ! "You can't go that way."
               3: "Du kan ikke klatre op på ", (the) x1, ".";
               4: "Du kan ikke klatre ned på ", (the) x1, ".";
               5: "Det blir vanskeligt, ", (the) x1, " er i vejen.";
               6: "Det kan du ikke, ", (the) x1," leder ingen steder.";
           }
  Insert:  switch(n)
           {   1: "Du må holde ", (the) x1,
                  " inden du kan lægge ", (DenEllerHan) x1,
                  " i noget.";
               2: print_ret (CDenEllerHan) x1, " kan ikke indeholde noget.";
               3: print_ret (The) x1, " er ",(IndefAdjClosed) x1,".";
               4: "Du må tage ", (DenEllerHam) x1, " af først.";
               5: "Du kan ikke lægge noget inden i sig selv.";
               6: "(tager ", (DenEllerHam) x1, " af først)^";
               7: "Der er ikke plads til mere i ", (the) x1, ".";
               8: "Udført.";
               9: "Du placerer ", (the) x1, " i ", (the) second, ".";
           }
  Inv:     switch(n)
           {   1: "Du har ingenting.";
               2: print "Du har";
               3: print ":^";
               4: print ".^";
           }
  Jump:           "Du hopper lidt på stedet.";
  JumpOver, Tie:  "Det ville ikke være til nogen særlig nytte.";
  Kiss:           "Du drømmer dig bort et øjeblik.";
  Listen:         "Du hører ikke noget uventet.";
  ListMiscellany:
           switch(n)
           {   1: print " (lyser)";
               2: print " (",(IndefAdjClosed) x1,")";
               3: print " (som er ",(IndefAdjClosed) x1," og lyser)";
               4: print " (",(IndefAdjEmpty) x1,")";
               5: print " (som er ",(IndefAdjEmpty) x1," og lyser)";
               6: print " (som er ",(IndefAdjClosed) x1," og ",(IndefAdjEmpty) x1,")";
               7: print " (som er ",(IndefAdjClosed) x1,", ",(IndefAdjEmpty) x1," og lyser)";
               8: print " (som lyser og er ",(IndefAdjWorn) x1;
               9: print " (lyser";
              10: print " (",(IndefAdjWorn) x1;
              11: print " (som ";
              12: print "er ", (IndefAdjOpen) x1;
              13: print "er ", (IndefAdjOpen) x1," men ",(IndefAdjEmpty) x1;
              14: print "er ", (IndefAdjClosed) x1;
              15: print "er ", (IndefAdjClosed) x1," og ",(IndefAdjLocked) x1;
              16: print " og ",(IndefAdjEmpty) x1;
              17: print " (som er ",(IndefAdjEmpty) x1,")";
              18: print " indeholder ";
              19: print " (på ";
              20: print ", oven ";
              21: print " (i ";
              22: print ", inden ";
           }
  LMode1:         " er nu i ~normal~ udskriftsmodus, som giver lange beskrivelser af steder, du besøger for første gang og ellers korte beskrivelser.";
  LMode2:         " er nu i ~lang~ udskriftsmodus, som altid giver lange beskrivelser af steder (selv om du har besøgt dem tidligere).";
  LMode3:         " er nu i ~kort~ udskriftsmodus, som altid giver korte beskrivelser af steder (selv om du besøger dem for første gang).";
  Lock:    switch(n)
           {   1: print_ret (CDenEllerHan) x1, " virker ikke til at være noget, du kan låse.";
               2: print_ret (CDenEllerHan) x1, " er allerede ",(IndefAdjLocked) x1,".";
               3: "Du må først lukke ", (the) x1, ".";
               4: print_ret (CDenEllerHan) x1, " lader ikke til at passe til låsen.";
               5:  "Du låser ", (the) x1, ".";
           }
  Look:    switch(n)
           {   1: print " (på ", (the) x1, ")";
               2: print " (i ", (the) x1, ")";
               3: print " (som ", (object) x1, ")";
               4: print "^På ", (the) x1;
                  WriteListFrom(child(x1),
                    ENGLISH_BIT+RECURSE_BIT+PARTINV_BIT+TERSE_BIT+CONCEAL_BIT+ISARE_BIT);
                  ".";
               5,6:
                   if (x1 ~= location) {
                       if (x1 has supporter) print "^På "; else print "^I ";
                       print (the) x1, " kan du";
                   }
                   else print "^Du kan";
                   if (n==5) print " også";
                   print " se ";
                   WriteListFrom(child(x1),
                     ENGLISH_BIT+RECURSE_BIT+PARTINV_BIT+TERSE_BIT+CONCEAL_BIT+WORKFLAG_BIT);
                   if (x1 ~= location) ".";
                   ".";
               7:  "Du ser ikke noget specielt i den retning.";
           }
  LookUnder: switch(n)
           {   1: "Men det er jo mørkt.";
               2: "Du finder ingenting af interesse.";
           }
  Mild:           "Akkurat, det tænkte jeg også.";
  Miscellany:
           switch(n)
           {   1: "(tager blot hensyn til de første seksten ting)^";
               2: "Ingenting at gøre!";
               3: print " Du er død ";
               4: print " Du har vundet ";
               5: print "^Du kan vælge: GENSTART, HENT et gemt spil";
                  #IFDEF DEATH_MENTION_UNDO;
                  print ", FORTRYD dit sidste træk";
                  #ENDIF;
                  if (TASKS_PROVIDED==0)
                      print ", vis FULL information om dine point";
                  if (deadflag==2 && AMUSING_PROVIDED==0)
                      print ", se nogle forslag på AMUSING ting at prøve";
                  " eller AFSLUT?";
               6: "[Spiltolkeren støtter desværre ikke ~FORTRYD~.]";
               7: "~Fortryd~ fejlede. [Ikke alle spiltolkere understøtter ~FORTRYD~.]";
               8: "Vælg et af svarene ovenfor.";
               9: "^Nu er det mørkt her.";
              10: "Undskyld mig?";
              11: "[Du kan ikke fortryde noget, du ikke har gjort.]";
              12: "[Du kan ikke fortryde to gange i træk.]";
              13: "[Sidste træk fortrudt.]";
              14: "Det kan desværre ikke rettes op.";
              15: "Det var da så lidt.";
              16: "~UPS~ kan kun rette et enkelt ord op.";
              17: "Det er mørkt, og du kan ikke se noget som helst.";
              18: print "dig selv";
              19: "Lige så nydelig som altid.";
              20: "For at gentage en kommando som ~frø, hopp~, skriv bare
                   ~IGEN~, ikke ~frø, IGEN~.";
              21: "Det kan du nok ikke gentage.";
              22: "Du kan ikke starte en kommando med et komma.";
              23: "Det virker som du taler med nogen, men jeg forstår ikke med hvem.";
              24: "Du kan ikke tale med ", (the) x1, ".";
              25: "For at tale med nogen, prøv ~person, hej~ eller noget lignende.";
              26: "(tager først ", (the) not_holding, " op)";
              27: "Den sætning forstod jeg ikke.";
              28: print "Måske mente du noget i stil med ~";
              29: "Det tal forstod jeg ikke.";
              30: "Du ser ikke noget sådant her.";
              31: "Det virker som du har sagt for lidt.";
              32: "Du har ikke ", (CDenEllerHam) x1, "."; ! ¤#¤ Could be rephrased to include the correct Den/Det, using ThatorThose?
              33: "Du kan ikke bruge det verbum på flere ting.";
              34: "Du kan kun nævne flere ting én gang per linje.";
              35: "Jeg er ikke sikker på hvad ~", (address) pronoun_word,
                  "~ refererer til.";
              36: "Du udelod noget, som alligevel ikke var med.";
              37: "Det giver mest mening med noget levende.";
              38: "Det verbum kender jeg ikke.";
              39: "Det er ikke noget, du har brug for i dette spil.";
              40: "Du kan ikke se ~", (address) pronoun_word,
                  "~ (", (the) pronoun_obj, ") lige nu.";
              41: "Jeg forstod ikke rigtig den afslutning.";
              42: print "Der er ";
                  if (x1==0) print "ingen";
                  else { print "kun "; LanguageNumber(x1, 0, multiple_object-->1); }
                  if (x1==1) {
                    if(multiple_object-->1 has neuter)
                      " tilgængeligt.";
                    " tilgængelig.";
                  } else
                    " tilgængelige."; ! ¤#¤ Tries to care about gender
              43: "Ingenting at gøre.";
              44: switch(action_to_be) {
                    ##Drop, ##Insert, ##PutOn: "Du har ingenting at slippe.";
                    ##Take: "Du kan ikke tage alt på én gang.";
                    default: "Der er ingenting at tage.";
                  }
              45: print "Hvad mener du, ";
              46: print "Mener du ";
              47: "Du kan kun vælge én ting her. Præcis hvilken mener du?";
              48: print "Jeg antager, at du mener ~"; ! ¤#¤ This message should work now
                  if (actor~=player) print (name) actor, ", ";
                  PrintCommand();
                  style bold;
                  print " nogen eller noget";
                  style roman;
                  print "~, men hvem eller hvad?^";
              49: print "Jeg antager, at du mener ~";
                  if (actor~=player) print (the) actor, ", ";
                  PrintCommand();
                  style bold;
                  print " nogen eller noget";
                  style roman;
                  print "~, men hvem eller hvad?^";
              50: print "Du ";
                  if (x1 > 0) print "fik"; else { x1 = -x1; print "mistede"; }
                  ! print " akurat ";
                  print " ";
                  LanguageNumber(x1,false,false,neuter);
                  print " point";
              51: "(eftersom noget dramatisk er sket, er din liste af kommandoer blevet afbrudt)";
              52: "^Skriv et tal mellem 1 og ", x1,
                  ", 0 for at vise dette igen eller tryk ENTER.";
              53: "^[Tryk MELLEMRUM.]";
              54: "[Kommentar gemt.]";
              55: "[Kommentar IKKE gemt.]";
              56: "~?";
              57: print "?^";
           }
  NotifyOn:       "Pointsbeskeder til.";
  NotifyOff:      "Pointbeskeder fra.";
  Objects: switch(n)
           {   1: "Objekter du har benyttet:^";
               2: "Ingen.";
               3: print "   (",(IndefAdjWorn) x1,")"; ! ¤#¤ Is x1 actually set to the object# in question here? (It is now)
               4: print "   (",(indefadjheld) x1,")"; ! ¤#¤ Is x1 actually set to the object# in question here? (It is now)
               5: print "   (",(indefadjgiven) x1,")"; ! ¤#¤ Is x1 actually set to the object# in question here? (It is now)
               6: print "   (i ", (name) x1, ")";
               7: print "   (i ", (the) x1, ")";
               8: print "   (indeni ", (the) x1, ")";
               9: print "   (på ", (the) x1, ")";
              10: print "   (borte)";
           }
  Open:    switch(n)
           {   1: print_ret (CDenEllerHan) x1,
                  " kan ikke åbnes.";
               2: print_ret (CDenEllerHan) x1," virker ",(IndefAdjLocked) x1,".";
               3: print_ret (CDenEllerHan) x1," er allerede ",(IndefAdjOpen) x1,".";
               4: print "Du åbner ", (the) x1, " og finder ";
                  if (WriteListFrom(child(x1),
                      ENGLISH_BIT + TERSE_BIT + CONCEAL_BIT)==0) "ingenting.";
                  ".";
               5: "Du åbner ", (the) x1, ".";
           }
  Order:          print_ret (The) x1," har vigtigere ting at gøre.";
  Places: switch (n) {
        1:  print "Steder du har besøgt:^";
        2:  print ".^";
    }
  Pray:           "Intet håndgribeligt kommer ud af din bøn.";
  Pronouns: switch(n)
           {   1: print "Lige nu refererer ";
               2: print "til ";
               3: print "ikke til noget specielt";
               4: "ingen pronomen til noget som helst.";
               5: ".";
           }
  Pull, Push, Turn:
           switch(n)
           {   1: print_ret (CDenEllerHan) x1, " sidder fast.";
               2: "Det mislykkedes.";
               3: "Ingenting ser ud til at ske.";
               4: "Det ville være direkte uhøfligt.";
           }
  PushDir: switch(n)
           {   1: "Er det det bedste du kommer på?";
               2: "Det er ikke nogen retning.";
               3: "Det går nok ikke i den retning.";
           }
  PutOn:   switch(n)
           {   1: "Du må holde ", (the) x1,
                  " før du kan lægge ", (DenEllerHam) x1,
                  " på noget.";
               2: "Du kan ikke lægge noget på sig selv.";
               3: "At lægge ting på ", (the) x1, " kommer der nok ikke noget ud af.";
               4: "Du er ikke smidig nok.";
               5: "(tager ", (DenEllerHam) x1, " af først)^";
               6: "Det er ikke plads til noget mere på ", (the) x1, ".";
               7: "Udført.";
               8: "Du placerer ", (the) x1, " på ", (the) second, ".";
           }
  Quit:    switch(n)
           {   1: print "Svar ja eller nej.";
               2: print "Er du sikker på, at du vil afslutte? ";
           }
  Remove:  switch(n)
           {   1: print_ret (CDenEllerHan) x1, " er desværre ",(IndefAdjClosed) x1, ".";
               2: print_ret "Men ", (DenEllerHan) x1, " er ikke der nu.";
               3: "Fjernet.";
           }
  Restart: switch(n)
           {   1: print "Er du sikker på, at du vil genstarte? ";
               2: "Genstart fejlede.";
           }
  Restore: switch(n)
           {   1: "Hentning af spil fejlede.";
               2: "OK.";
           }
  Rub:            "Det giver ikke noget særlig resultat.";
  Save:    switch(n)
           {   1: "Gemning af spil fejlede.";
               2: "OK.";
           }
  Score:   switch (n) {
               1: if (deadflag) print "I dette spil fik du ";
                  else print "Du har hidtil fået ";
                  print score, " point af ", MAX_SCORE,
                  " mulige på ", turns, " træk";
                  return;
               2: "Det er ingen point i dette spil.";
           }
  ScriptOn: switch(n)
           {   1: "Udskrift er allerede i gang.";
               2: "Her starter en udskrift af";
               3: "Start af udskrift fejlede.";
           }
  ScriptOff: switch(n)
           {   1: "Udskrift er allerede slået fra.";
               2: "^Her slutter udskriften.";
               3: "Stop af udskrift fejlede.";
           }
  Search:  switch(n)
           {   1: "Men det er jo mørkt.";
               2: "Der er ingenting på ", (the) x1, ".";
               3: print "På ", (the) x1;
                  WriteListFrom(child(x1),
                      TERSE_BIT + ENGLISH_BIT + ISARE_BIT + CONCEAL_BIT);
                  ".";
               4: "Du finder ingenting af interesse.";
               5: "Du ser ikke, hvad som er i ", (the) x1, ", eftersom ",
                  (DenEllerHan) x1, " er ",(IndefAdjClosed) x1,".";
               6: print_ret (The) x1, " er ",(IndefAdjEmpty) x1,".";
               7: print "I ", (the) x1;
                  WriteListFrom(child(x1),
                      TERSE_BIT + ENGLISH_BIT + ISARE_BIT + CONCEAL_BIT);
                  ".";
           }
  Set:            "Nej, du kan ikke sætte ", (DenEllerHam) x1, ".";
  SetTo:          "Nej, du kan ikke sætte ", (DenEllerHam) x1, " til noget.";
  Show:    switch(n)
           {   1: "Du har ikke ", (the) x1, ".";
               2: print (The) x1, " er ikke imponer";
                  if (x1 has pluralname) "ende.";
                  else "et.";
           }
  Sing:           "Du har ikke meget af en sangstemme.";
  Sleep:          "Du føler dig ikke særlig søvnig.";
  Smell:          "Du lugter ikke noget uventet.";
  Sorry:          "Det er i orden. For denne gang.";
  Squeeze: switch(n)
           {   1: "Ikke pille!";
               2: "Det giver ikke noget mærkbart resultat.";
           }
  Strong:         "Ægte eventyrere taler ikke sådan.";
  Swim:           "Det er ikke til at svømme i.";
  Swing:          "Der er ingenting fornuftig at svinge her.";
  SwitchOn: switch(n)
           {   1: print_ret (CDenEllerHan) x1," er ikke noget du kan tænde.";
               2: print_ret (CDenEllerHan) x1,
                  " er allerede tændt.";
               3: "Du tænder for ", (the) x1, ".";
           }
  SwitchOff: switch(n)
           {   1: print_ret (CDenEllerHan) x1," er ikke noget du kan slukke.";
               2: print_ret (CDenEllerHan) x1,
                  " er allerede slukket.";
               3: "Du slukker for ", (the) x1, ".";
           }
  Take:    switch(n)
           {   1: print "Du tager "; LanguagePrintShortName(noun); ".";
               2: "Du er noget selvoptaget.";
               3: "Det vil ",(DenEllerHan) x1," nok ikke.";
               4: print_ret "Da må du først forlade ",(the) x1, ".";
               5: "Du har ", (DenEllerHam) x1, " allerede.";
               6: print_ret (CDenEllerHan) noun, " ser ud til at tilhøre ", (the) x1, ".";
               7: print_ret (CDenEllerHan) noun, " ser ud til at være en del af ", (the) x1, ".";
               8: print_ret "Du kommer ikke til ",(DenEllerHam) x1, ".";
               9: print_ret (The) x1, " er ikke ",(IndefAdjOpen) x1, ".";
              10: print_ret "Du kan næppe flytte ", (DenEllerHam) x1, ".";
              11: print_ret (CDenEllerHan) x1, " sidder fast.";
              12: "Du bærer allerede på for meget.";
              13: "(lægger ", (the) x1, " i ", (the) SACK_OBJECT,
                  " for at få plads)";
           }
  Taste:          "Du smager intet uventet.";
  Tell:    switch(n)
           {   1: "Du taler lidt med dig selv.";
               2: "Ingen reaktion.";
           }
  Think:          "Du grubler lidt.";
  ThrowAt: switch(n)
           {   1: "Meningsløst.";
               2: "Da det kommer til stykket, tør du ikke.";
           }
  Touch:   switch(n)
           {   1: "Fingrene væk!";
               2: "Det føles omtrent som forventet.";
               3: "Hvis du tror det hjælper, så vær så god.";
           }
  TranscribeOn:
           switch(n)
           {   1: "Transkriptionsmodus er allerede slået til.";
               2: "Transkriptionsmodus slået til. ~aa~ betyder nu å, ~ae~
               betyder æ og ~oe~ betyder ø. Skriv et udråbstegn før enhver
               sådan bogstavkombination, som ikke skal transkriberes, som
               f.eks. i ~aaben !poesibog~. Skriv ~transkription vis~ for at få
               besked, når transkriptionen bliver brugt.";
           }
  TranscribeOff:
           switch(n)
           {   1: "Transtriptionsmodus er allerede slået fra.";
               2: "Transtriptionsmodus slået fra.";
           }
  TranscribeShow:
           switch(n)
           {   1: "Transtriptionsmodus med visning er allerede slået til.";
               2: "Transtriptionsmodus med visning slået til. ~aa~ betyder nu
               å, ~ae~ betyder æ og ~oe~ betyder ø. Skriv et udråbsregn før
               enhver sådan bogstavkombination, som ikke skal transkriberes,
               som f.eks. i ~aaben !poesibog~.";
           }
  Unlock:  switch(n)
           {   1: print_ret (CDenEllerHan) x1, " ser ikke ud til at være noget, du kan låse op.";
               2: print_ret (CDenEllerHan) x1," er allerede u",(IndefAdjLocked) x1,".";
               3: print_ret (CDenEllerHan) x1, " ser ikke ud til at passe til låsen.";
               4: "Du låser ", (the) x1, " op.";
           }
  VagueGo:       "Du må sige, hvilken retning du vil gå.";
  Verify:  switch(n)
           {   1: "Spilfilen blev testet og ser ud til at være intakt.";
               2: "Spilfilen blev testet og ser ikke ud til at være intakt. Den kunne være korrupt.";
           }
  Wait:           "Du venter.";
  Wake:           "Dette er desværre ingen drøm.";
  WakeOther:      "Det virker unødvendig.";
  Wave:    switch(n)
           {   1: "Men du holder ikke i ", (DenEllerHan) x1, ".";
               2: "Du ser lidt fjollet ud, som du vifter med ", (the) x1, ".";
           }
  WaveHands:      "Du vifter med armene og føler dig lidt fjollet.";
  Wear:    switch(n)
           {   1: "Du kan ikke tage ", (DenEllerHam) x1, " på.";
               2: "Du har ikke ", (DenEllerHam) x1, ".";
               3: print_ret (CDenEllerHan) x1, " har du allerede på.";
               4: "Du tager ", (the) x1, " på.";
           }
  Yes, No:        "Det var et retorisk spørsmål.";
];

! ==============================================================================

Constant LIBRARY_DANISH;       ! for dependency checking.

! ==============================================================================
