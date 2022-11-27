! ----------------------------------------------------------------------------
!  GRAMMAR:  Grammar table entries for the standard verbs library.
!
!   Supplied for use with Inform 6 -- Release 6/11 -- Serial number 040227
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
!   In your game file, Include these library files in this order:
!       Include "DanAlpha"; ! Must be included at the *very* beginning of game
!                           ! even before Story and Headline are defined
!       Include "Parser";
!       Include "VerbLib";
!       Include "DanRout";
!       Include "DanGram";
!
! ==============================================================================

System_file;

#ifndef LIBRARY_DANISH;
  #ifdef LIBRARY_ENGLISH;
    Message "ERROR: You must use 'inform +language_name=Danish ...' to compile.";
  #ifnot;
    Message "ERROR: Files MUST be included in this order: Parser, Verblib, DanRout, DanishG.";
    abort;
  #endif;
#endif;

! ----------------------------------------------------------------------------
!  The "meta-verbs", commands to the game rather than in the game, come first:
! ----------------------------------------------------------------------------

Verb meta 'brief' 'normal' 'normale'
    *                                           -> LMode1
    * 'beskrivelser'                            -> LMode1;

Verb meta 'verbose' 'long' 'lang' 'lange'
    *                                           -> LMode2
    * 'beskrivelser'                            -> LMode2;

Verb meta 'superbrief' 'short' 'kort' 'korte'
    *                                           -> LMode3
    * 'beskrivelser'                            -> LMode3;

Verb meta 'notify' 'meddelelser'
    *                                           -> NotifyOn
    * 'on'                                      -> NotifyOn
    * 'off'                                     -> NotifyOff
    * 'til'                                     -> NotifyOn
    * 'fra'                                     -> NotifyOff;

Verb meta 'pronouns' 'nouns' 'pronomen' 'substantiv'
    *                                           -> Pronouns;

Verb meta 'quit' 'q//' 'afslut' 'a//' 'die' 'dø'
    *                                           -> Quit;

Verb meta 'recording' 'indspilning'
    *                                           -> CommandsOn
    * 'on'                                      -> CommandsOn
    * 'off'                                     -> CommandsOff
    * 'til'                                     -> CommandsOn
    * 'fra'                                     -> CommandsOff;

Verb meta 'replay' 'afspil'
    *                                           -> CommandsRead;

Verb meta 'restart' 'genstart'
    *                                           -> Restart;

Verb meta 'restore' 'hent' 'load'
    *                                           -> Restore
    * 'spil'                                    -> Restore
    * 'spillet'                                 -> Restore
    * 'gemt' 'spil'                             -> Restore;

Verb meta 'save' 'gem'
    *                                           -> Save
    * 'spil'                                    -> Save
    * 'spillet'                                 -> Save
    * 'gemt' 'spil'                             -> Save;

Verb meta 'score' 'point'
    *                                           -> Score;

Verb meta 'full' 'fuld' 'fullscore' 'fuldpoint'
    *                                           -> FullScore
    * 'score'                                   -> FullScore;

Verb meta 'script' 'transcript' 'udskrift'
    *                                           -> ScriptOn
    * 'on'                                      -> ScriptOn
    * 'off'                                     -> ScriptOff
    * 'til'                                     -> ScriptOn
    * 'fra'                                     -> ScriptOff;

Verb meta 'noscript' 'unscript'
    *                                           -> ScriptOff;

Verb meta 'transkriber' 'transkription'
    *                                           -> TranscribeOn
    * 'til'                                     -> TranscribeOn
    * 'vis'/'visning'                           -> TranscribeShow
    * 'fra'                                     -> TranscribeOff;

Verb meta 'verify' 'verificer'
    *                                           -> Verify;

Verb meta 'version'
    *                                           -> Version;

#Ifndef NO_PLACES;
Verb meta 'objects' 'objekter' 'ting'
    *                                           -> Objects;
Verb meta 'places' 'steder'
    *                                           -> Places;
#Endif; ! NO_PLACES

! ------------------------------------------------------------------------------
!  Debugging grammar
! ------------------------------------------------------------------------------


#Ifdef DEBUG;
Verb meta 'actions'
    *                                           -> ActionsOn
    * 'on'                                      -> ActionsOn
    * 'off'                                     -> ActionsOff;
Verb meta 'changes'
    *                                           -> ChangesOn
    * 'on'                                      -> ChangesOn
    * 'off'                                     -> ChangesOff;
Verb meta 'gonear'
    * noun                                      -> Gonear;
Verb meta 'goto'
    * number                                    -> Goto;
Verb meta 'random'
    *                                           -> Predictable;
Verb meta 'routines' 'messages'
    *                                           -> RoutinesOn
    * 'on'                                      -> RoutinesOn
    * 'off'                                     -> RoutinesOff;
Verb meta 'scope'
    *                                           -> Scope
    * noun                                      -> Scope;
Verb meta 'showobj'
    *                                           -> Showobj
    * number                                    -> Showobj
    * multi                                     -> Showobj;
Verb meta 'showverb'
    * special                                   -> Showverb;
Verb meta 'timers' 'daemons'
    *                                           -> TimersOn
    * 'on'                                      -> TimersOn
    * 'off'                                     -> TimersOff;
Verb meta 'trace'
    *                                           -> TraceOn
    * number                                    -> TraceLevel
    * 'on'                                      -> TraceOn
    * 'off'                                     -> TraceOff;
Verb meta 'abstract'
    * noun 'to' noun                            -> XAbstract;
Verb meta 'purloin'
    * multi                                     -> XPurloin;
Verb meta 'tree'
    *                                           -> XTree
    * noun                                      -> XTree;
Verb meta 'falkeblik'
    *                                           -> CheckGame
    * noun                                      -> CheckGame
    * number                                    -> CheckGame
    * 'hvilke'                                  -> CheckGameSelect;

#Ifdef TARGET_GLULX;
Verb meta 'glklist'
    *                                           -> Glklist;
#Endif; ! TARGET_

#Endif; ! DEBUG

! ----------------------------------------------------------------------------
!  And now the game verbs.
! ----------------------------------------------------------------------------

[ Exitable ;
  if(player in noun) rtrue;
  rfalse;
];

[ ADirection; if (noun in compass) rtrue; rfalse; ];

Verb 'angrib' 'bræk' 'knæk' 'ødelæg' 'bekæmp' 'dræb'
     'myrd' 'kvæst' 'kværk' 'smadre' 'smadr' 'kvas' 'torturer'
                * noun                                      -> Attack
                * noun 'med' held                           -> Attack;
Verb 'slås' 'kæmp'
                * 'med' noun                                -> Attack;
Verb 'bed'
                *                                           -> Pray
                * 'om' 'at' 'få' noun 'af' creature         -> AskFor reverse
                * 'om' noun 'af' creature                   -> AskFor reverse
                * creature 'om' noun                        -> AskFor
                * creature 'om' 'at' 'give' 'mig' noun      -> AskFor
                * creature 'give' 'mig' noun                -> AskFor;
Verb 'befal' 'anmod' 'beordre'
                * creature 'om' noun                        -> AskFor
                * creature 'om' 'at' 'give' 'mig' noun      -> AskFor
                * creature 'giv' 'mig' noun                 -> AskFor;
Verb 'fortæl' 'forklar' 'bekend' 'beret' 'nævn'
                * 'for' creature 'om' topic                 -> Tell
                * creature 'om' topic                       -> Tell
                * topic 'for' creature                      -> Tell reverse
                * 'om' topic 'for' creature                 -> Tell reverse;
Verb 'tal' 'snak'
                * 'om' topic 'for'/'til' creature           -> Tell reverse
                * 'til' creature 'om' topic                 -> Tell;
Verb 'beskær'
                * noun                                      -> Cut;
Verb 'bind' 'fæst' 'knyt'
                * noun                                      -> Tie
                * noun 'fast' 'i//' noun                    -> Tie
                * noun 'i//' noun                           -> Tie
                * noun 'til' noun                           -> Tie;
Verb 'blæs' 'pust'
                * noun                                      -> Blow
                * 'på'/'i//'/'til' noun                     -> Blow
                * 'på'/'i//'/'til' held                     -> Blow;
Verb 'brænd' 'tænd' 'antænd'
                * 'for'/'på' noun                           -> SwitchOn
                * noun                                      -> SwitchOn
                * noun 'med' held                           -> Burn;
Verb 'bær'
                * clothing                                  -> Wear;
Verb 'træk' 'hiv'
                * 'i//' noun                                -> Pull
                * noun                                      -> Pull;
Verb 'drik'
                * noun                                      -> Drink
                * 'af' noun                                 -> Drink;
Verb 'dump' 'smid'
                * multiheld                                 -> Drop
                * multiexcept 'i//'/'indeni' noun           -> Insert
                * multiexcept 'ind' 'i//' noun              -> Insert
                * multiexcept 'på'/'ovenpå' noun            -> PutOn
                * multiexcept 'mod' noun                    -> ThrowAt
                * multiexcept 'op'/'oppe' 'på' noun         -> PutOn;
Verb 'flyt'
                * noun                                      -> Push
                * 'på' noun                                 -> Push
                * noun noun                                 -> PushDir
                * noun 'til' noun                           -> Transfer;
Verb 'spørg'
                * creature 'om' topic                       -> Ask
                * 'om' 'at' 'få' noun 'af' creature         -> AskFor reverse;
Verb 'fyld'
                * noun                                      -> Fill
                * noun 'op'                                 -> Fill;
Verb 'giv' 'tilbyd' 'byd' 'betal'
                * held 'til' creature                       -> Give
                * held 'bort' 'til' creature                -> Give
                * creature held                             -> Give reverse;
Verb 'gennemsøg'
                * noun                                      -> Search;
Verb 'gnid' 'poler' 'polere' 'knub' 'stryg' 'skrub' 'puds'
                * noun                                      -> Rub
                * 'af'/'på' noun                            -> Rub;
Verb 'grav'
                * noun                                      -> Dig
                * noun 'med' held                           -> Dig
                * noun 'op' 'med' held                      -> Dig
                * 'i//'/'op' noun 'med' held                -> Dig;
Verb 'gyng' 'gynge' 'sving'
                * noun                                      -> Swing
                * 'i//' noun                                -> Swing
                * 'på' noun                                 -> Swing;
Verb 'gå' 'løb' 'sprint'
                *                                           -> VagueGo
                * 'ind'                                     -> GoIn
                * 'ud'                                      -> Exit
                * noun=ADirection                           -> Go
                * 'til'/'mod' noun=ADirection               -> Go
                * 'op' 'fra' noun                           -> Exit
                * noun                                      -> Enter
                * 'på' noun                                 -> Enter
                * 'op' 'på' noun                            -> Enter
                * 'ind' 'i//'/'gennem'/'igennem' noun       -> Enter
                * 'gennem'/'igennem' noun                   -> Enter
                * 'af' noun                                 -> Exit
                * 'ud' 'af' noun                            -> Exit
                * 'ud' 'i//'/'gennem'/'igennem' noun        -> Exit
                * 'ned' 'fra' noun                          -> Exit;
Verb 'stig'
                * 'på' noun                                 -> Enter
                * 'op'                                      -> Exit
                * 'op' 'fra' noun                           -> Exit
                * 'ind'                                     -> GoIn
                * 'ud'                                      -> Exit
                * 'op' 'på' noun                            -> Enter
                * 'ind' 'i//'/'gennem'/'igennem' noun       -> Enter
                * 'af' noun                                 -> Exit
                * 'ud' 'af' noun                            -> Exit
                * 'ud' 'i//'/'gennem'/'igennem' noun        -> Exit
                * 'ned' 'fra' noun                          -> Exit;
Verb 'hop' 'spring'
                *                                           -> Jump
                * 'over' noun                               -> JumpOver;
Verb 'ind'
                *                                           -> GoIn;
Verb 'ja' 'j//' 'ok' 'okay' 'jep' 'jo' 'alright' 'javel' 'yes' 'y//'
                *                                           -> Yes;
Verb 'kast'
                * multiheld                                 -> Drop
                * multiexcept 'i//'/'ind' noun              -> Insert
                * multiexcept 'ind' 'i//' noun              -> Insert
                * multiexcept 'i//'/'indeni' noun           -> Insert
                * multiexcept 'inden' 'i//' noun            -> Insert
                * multiexcept 'på'/'ovenpå' noun            -> ThrowAt
                * multiexcept 'op'/'oppe' 'på' noun         -> ThrowAt
                * held 'på'/'mod' noun                      -> ThrowAt
                * held 'til' creature                       -> ThrowAt;
Verb 'klem'
                * creature                                  -> Kiss
                * 'på' creature                             -> Kiss
                * noun                                      -> Squeeze
                * 'på' noun                                 -> Squeeze;
Verb 'klatre' 'klatr' 'kravl'
                * 'på'/'over'/'opover'/'i//' noun           -> Climb
                * 'ud'/'af'/'ned'                           -> Exit
                * 'af'/'ned' 'fra' noun                     -> Exit
                * 'op' 'på' noun                            -> Climb
                * 'ind' 'i//'/'gennem'/'igennem' noun       -> Enter
                * 'gennem'/'igennem' noun                   -> Enter;
Verb 'kryds'
                * noun                                      -> Enter;
Verb 'omfavn' 'omfavne' 'kram' 'knus'
                * creature                                  -> Kiss
                * noun                                      -> Squeeze;
Verb 'kys'
                * creature                                  -> Kiss;
Verb 'køb' 'handle' 'erhverv'
                * 'ind' noun                                -> Buy
                * noun                                      -> Buy;
Verb 'undskyld' 'beklager' 'sorry'
                * 'mig'                                     -> Sorry
                *                                           -> Sorry;
Verb 'status' 'ejendele' 'habengut' 'beholdning' 'pikpak' 'i//' 't//' 'list'
                *                                           -> Inv
                * 'ejendele'/'ting'                         -> Inv
                * 'mine' 'ejendele'/'ting'                  -> Inv
                * 'lang'                                    -> InvTall
                * 'bred'                                    -> InvWide;
Verb 'luk'
                * noun                                      -> Close
                * noun 'igen'                               -> Close
                * 'af' 'for' noun                           -> SwitchOff;
Verb 'lugt' 'snif' 'snus'
                *                                           -> Smell
                * noun                                      -> Smell
                * 'til'/'i//' noun                          -> Smell;
Verb 'lyt' 'hør'
                *                                           -> Listen
                * 'på'/'til' noun                           -> Listen;
Verb 'læg'
                * multiheld                                 -> Drop
                * 'mig'/'dig' 'på'/'i//' noun               -> Enter
                * 'mig'/'dig' 'oppe' 'på'/'i//' noun        -> Enter
                * multiexcept 'indeni'/'i//' noun           -> Insert
                * multiexcept 'inden' 'i//' noun            -> Insert
                * multiexcept 'på'/'ovenpå' noun            -> PutOn
                * multiexcept 'oppe' 'på' noun              -> PutOn
                * multiheld 'ned'/'bort'                    -> Drop
                * multiheld 'ned'/'ind' 'i//'/'indeni' noun -> Insert
                * multiheld 'ned'/'ind' 'inden' 'i//' noun  -> Insert
                * multiexcept 'ned'/'op' 'på'/'ovenpå' noun -> PutOn
                * multiexcept 'ned'/'op' 'overn' 'på' noun  -> PutOn
                * 'fra' 'dig'/'mig' multiheld               -> Drop;

Verb 'efterlad'
                * multiheld                                 -> Drop
                * multiexcept 'i//'/'indeni' noun           -> Insert
                * multiexcept 'inde' 'i//' noun             -> Insert
                * multiexcept 'på'/'overnpå' noun           -> PutOn
                * multiexcept 'oppe' 'på' noun              -> PutOn;
Verb 'læs'
                * noun                                      -> Examine
                * 'i//'/'på' noun                           -> Examine
                * 'om' topic 'i//' noun                     -> Consult;
Verb 'lås'
                * noun 'med' held                           -> Lock
                * noun 'op' 'med' held                      -> Unlock;
Verb 'sgu' 'sørens' 'fy' 'pokker'
                *                                           -> Mild
                * topic                                     -> Mild;
Verb 'nej' 'næ'
                *                                           -> No;
Verb 'placer' 'put'
                * multiexcept 'i//'/'indeni' noun           -> Insert
                * multiexcept 'inde' 'i//' noun             -> Insert
                * multiexcept 'på'/'ovenpå' noun            -> PutOn
                * multiexcept 'oppe' 'på' noun              -> PutOn;
Verb 'rejs'
                * 'mig'/'dig'                               -> Exit
                * 'mig'/'dig' 'op'                          -> Exit
                * 'mig'/'dig' 'fra' noun                    -> Exit;
Verb 'rør' 'berør'
                * noun                                      -> Touch
                * 'på'/'ved' noun                           -> Touch;
Verb 'svøm' 'dyk'
                *                                           -> Swim;
Verb 'sid' 'lig'
                * 'på' noun                                 -> Enter
                * 'oppe'/'op' 'på' noun                     -> Enter
                * 'inde' 'i//' noun                         -> Enter
                * 'i//'/'indeni'/'på' noun                  -> Enter;
Verb 'syng' 'nyn' 'jodle'
                *                                           -> Sing;
Verb 'fanden' 'fandens' 'fandme' 'satan' 'satans' 'helvede' 'djævlens'
     'lort' 'pis' 'røv' 'knep' 'pul' 'forpulede' 'fuck'
                *                                           -> Strong
                * topic                                     -> Strong;
Verb 'skær'
                * noun                                      -> Cut
                * noun 'ned'                                -> Cut
                * 'af'/'op'/'ned'/'i//' noun                -> Cut;
Verb 'slå'
                * topic 'op' 'i//' noun                     -> Consult
                * noun                                      -> Attack
                * noun 'med' held                           -> Attack
                * noun 'i//' 'stykker'                      -> Attack
                * switchable 'til'                          -> SwitchOn
                * switchable 'fra'                          -> SwitchOff
                * 'til'/'på' noun                           -> Attack;
Verb 'slip'
                * multiheld                                 -> Drop
                * multiexcept 'på'/'ovenpå' noun            -> PutOn
                * multiexcept 'oven' 'på' noun              -> PutOn
                * multiexcept 'i//'/'indeni' noun           -> Insert
                * multiexcept 'inden' 'i//' noun            -> Insert
                * multiexcept 'på'/'ovenpå' noun            -> PutOn
                * multiexcept 'oven' 'på' noun              -> PutOn
                * multiexcept 'i//'/'indeni' noun           -> Insert
                * multiexcept 'inde' 'i//' noun             -> Insert;
Verb 'sluk'
                * noun                                      -> SwitchOff
                * 'for'/'på' noun                           -> SwitchOff;
Verb 'smag' 'sut' 'slik' 'sug'
                * noun                                      -> Taste
                * 'på' noun                                 -> Taste;
Verb 'spark'
                * noun                                      -> Attack
                * noun 'i//' 'stykker'                      -> Attack;
Verb 'kæl' 'kærtegn' 'føl' 'hold'
                * noun                                      -> Touch
                * 'på'/'med' noun                           -> Touch;
Verb 'beføl'
                * noun                                      -> Touch;
Verb 'snur' 'skru' 'vrid' 'drej'
                * noun                                      -> Turn
                * 'på' noun                                 -> Turn
                * noun 'på'/'af'                            -> Turn
                * 'på' switchable                           -> SwitchOn
                * switchable 'af'                           -> SwitchOff;
Verb 'sov' 'hvil'
                *                                           -> Sleep;
Verb "start"
                * noun                                      -> SwitchOn
                * 'igen'                                    -> Restart;
Verb 'stå'
                *                                           -> Exit
                * 'af'                                      -> Exit
                * 'op'                                      -> Exit
                * 'på'                                      -> Enter
                * 'på' noun                                 -> Enter
                * 'i' noun                                  -> Enter;
Verb 'stil' 'indstil'
                * 'mig'/'dig' 'på'/'i//' noun               -> Enter
                * 'mig'/'dig' 'op'                          -> Exit
                * multiexcept 'på'/'ovenpå' noun            -> PutOn
                * multiexcept 'oven' 'på' noun              -> PutOn
                * multiheld 'ned'                           -> Drop
                * multiexcept 'ned'/'op' 'på'/'ovenpå' noun -> PutOn
                * multiexcept 'ned'/'op' 'oven' 'på' noun   -> PutOn
                * multiexcept 'ned'/'ind' 'i//'/'indeni' noun
                                                            -> Insert
                * multiexcept 'ned'/'ind' 'inden' 'i//' noun
                                                            -> Insert
                * multiexcept 'i//'/'indeni' noun           -> Insert
                * multiexcept 'inden' 'i//' noun            -> Insert
                * noun                                      -> Set
                * noun 'ind'                                -> Set
                * noun 'på'/'til' special                   -> SetTo
                * noun 'ind' 'på'/'til' special             -> SetTo;
Verb 'stop'
                * noun                                      -> SwitchOff;
Verb 'svar'
                * creature topic                            -> Answer;
Verb 'svælg'
                * noun                                      -> Drink;
Verb 'sig'
                * topic 'til' creature                      -> Answer;
Verb 'sæt'
                * 'mig'/'dig' 'på'/'i//' noun               -> Enter
                * 'mig'/'dig' 'oppe' 'på'/'i//' noun        -> Enter
                * noun 'fast'                               -> Tie
                * noun 'fast' 'i//' noun                    -> Tie
                * 'fast' noun 'i//' noun                    -> Tie
                * multiexcept 'på'/'ovenpå' noun            -> PutOn
                * multiexcept 'oven' 'på' noun              -> PutOn
                * multiheld 'ned'                           -> Drop
                * multiexcept 'ned'/'op' 'på'/'ovenpå' noun -> PutOn
                * multiexcept 'ned'/'op' 'oven' 'på' noun   -> PutOn
                * multiexcept 'ned'/'ind' 'i//'/'indeni' noun
                                                            -> Insert
                * multiexcept 'ned'/'ind' 'inden' 'i//' noun
                                                            -> Insert
                * multiexcept 'i//'/'indeni' noun           -> Insert
                * multiexcept 'inden' 'i//' noun            -> Insert
                * multiexcept 'i//'/'indeni' noun           -> Insert
                * multiexcept 'inde' 'i//' noun             -> Insert
                * switchable 'på'                           -> SwitchOn
                * clothing 'på'                             -> Wear
                * noun                                      -> Set
                * noun 'til'/'på' special                   -> SetTo;
Verb 'søg' 'led'
                * 'igennem'/'i//' noun                      -> Search
                * 'gennem' noun                             -> Search;
Verb 'tag'
                * multi                                     -> Take
                * 'fat' 'i//' multi                         -> Take
                * multi 'op'/'frem'                         -> Take
                * noun 'af'                                 -> Disrobe
                * noun 'af' 'mig'/'dig'                     -> Disrobe
                * worn 'af'                                 -> Disrobe
                * worn 'af' 'mig'/'dig'                     -> Disrobe
                * multiinside 'fra'/'ud'/'i//' noun         -> Remove
                * multiinside 'op'/'frem'/'ud'/'bort' 'i//'/'fra'/'udfra' noun
                                                            -> Remove
                * noun 'på' 'mig'/'dig'                     -> Wear
                * clothing 'på'                             -> Wear
                * clothing 'på' 'mig'/'dig'                 -> Wear
                * 'på' noun                                 -> Touch;
Verb 'stjæl'
                * multi                                     -> Take;
Verb 'fjern'
                * multi                                     -> Take
                * multiinside 'fra' noun                    -> Remove;
Verb 'se' 'kig' 'k//' 'l//'
                *                                           -> Look
                * 'dig'/'mig' 'rundt'/'omkring'             -> Look
                * noun=ADirection                           -> Examine
                * 'mod' noun=ADirection                     -> Examine
                * noun                                      -> Examine
                * 'på' noun                                 -> Examine
                * 'nærmere' 'på' noun                       -> Examine
                * 'i//'/'indeni'/'gennem'/'igennem' noun    -> Search
                * 'ind'/'inde' 'i//' noun                   -> Search
                * 'under' noun                              -> LookUnder;
Verb 'tryk' 'skub' 'pres'
                * noun                                      -> Push
                * 'på'/'til' noun                           -> Push
                * noun noun                                 -> PushDir
                * noun 'til' noun                           -> Transfer;
Verb 'tænk' 'gruble' 'funder'
                *                                           -> Think;
Verb 'tøm'
                * noun                                      -> Empty
                * noun 'ud'                                 -> Empty
                * noun 'ud'/'udad' 'i//'/'indeni'/'på'/'ovenpå' noun
                                                            -> EmptyT;
Verb 'undersøg' 'u//' 'examine' 'x//' 'gransk' 'studer'
                * noun                                      -> Examine;
Verb 'ud'
                *                                           -> Exit
                * 'af' noun                                 -> Exit;
Verb 'vågn'
                *                                           -> Wake
                * 'op'/'til'                                -> Wake;
Verb 'vift' 'vink'
                *                                           -> WaveHands
                * 'med' held                                -> Wave;
Verb 'vis' 'presenter'
                * held creature                             -> Show reverse
                * held 'for'/'til' creature                 -> Show;
Verb 'væk'
                * creature                                  -> WakeOther;
Verb 'roter' 'vend'
                * noun                                      -> Turn
                * 'rundt'/'på' noun                         -> Turn;
Verb 'vent' 'z//'
                *                                           -> Wait;
Verb 'spis' 'æd'
                * noun                                      -> Taste
                * held                                      -> Eat
                * held 'op'                                 -> Eat;
Verb 'åbn' 'åben'
                * noun                                      -> Open
                * noun 'med' held                           -> Unlock;

! ----------------------------------------------------------------------------
!  Final task: provide trivial routines if the user hasn't already:
! ----------------------------------------------------------------------------
#Stub TimePasses      0;
#Stub Amusing         0;
#Stub DeathMessage    0;
#Stub DarkToDark      0;
#Stub NewRoom         0;
#Stub LookRoutine     0;
#Stub AfterLife       0;
#Stub GamePreRoutine  0;
#Stub GamePostRoutine 0;
#Stub AfterPrompt     0;
#Stub BeforeParsing   0;
#Stub PrintTaskName   1;
#Stub InScope         1;
#Stub UnknownVerb     1;
#Stub PrintVerb       1;
#Stub ParserError     1;
#Stub ParseNumber     2;
#Stub ChooseObjects   2;

#ifdef TARGET_GLULX;
  #Stub IdentifyGlkObject 4;
  #Stub HandleGlkEvent    2;
  #Stub InitGlkWindow     1;
#endif; ! TARGET_GLULX

#IFNDEF PrintRank;
  Constant Make__PR;
#ENDIF;
#IFDEF Make__PR;
  [ PrintRank; "."; ];
#ENDIF;
#IFNDEF ParseNoun;
  Constant Make__PN;
#ENDIF;
#IFDEF Make__PN;
  [ ParseNoun obj; obj=obj; return -1; ];
#ENDIF;
#Default Story    0;
#Default Headline 0;
#IFDEF INFIX;
  #Include "infix";
#ENDIF;
! ==============================================================================

Constant LIBRARY_GRAMMAR;       ! for dependency checking

! ==============================================================================
