
\version "2.18.2"
% automatically converted by musicxml2ly from 1675666-Top_Gun_Theme.mxl

\header {
    encodingsoftware = "MuseScore 2.0.2"
    source = "http://api.musescore.com/score/1675666"
    encodingdate = "2016-01-26"
    title = "Top Gun Theme "
    }

#(set-global-staff-size 20.0750126457)
\paper {
    paper-width = 21.59\cm
    paper-height = 27.94\cm
    top-margin = 1.0\cm
    bottom-margin = 2.0\cm
    left-margin = 1.0\cm
    right-margin = 1.0\cm
    }
\layout {
    \context { \Score
        autoBeaming = ##f
        }
    }


CounterpointVoice = {
	\clef bass \key g \major \time 4/4
	r8 g,8 c8 b,8 cis'8 b8 ~ b16 aes16 ais,16 gis,16
	b16 aes16 a,16 gis,16 ~ gis,8 f,8 gis,8 f,8 ~ f,16 ees,16
	gis16 f16 a8 g8 ~ g16 f16 a,16 g,16 bes,8 aes,8
	a16 gis16 f16 ees16 a,8 gis,8 ~ gis,4. fis,4. ~ fis,8 ees,8
	cis'8 b8 c'8 bes8 ~ bes16 aes16 ais,16 gis,16 b,16 a,16
	~ a,16 g,16 ~ g,8 f,8 g,8 f,8 ~ f,16 ees,16 gis,16 fis,16
	a8 g8 ~ g16 fis16 a,16 gis,16 bes8 a8 ~ a16 g16
	fis,16 ees,16 c2 b,2 fis8 e8 cis8 b,8 cis'8 bes8
	~ bes16 aes16 ais,16 gis,16 b16 a16 ~ a16 gis16 ~ gis8 f8
	gis8 f8 ~ f16 ees16 g,16 fis,16 a8 g8 ~ g16 f16
	a16 g16 bes8 a8 ~ a16 gis16 f16 e16 a8 gis8
	~ gis8 fis8 g4 fis4 ~ fis8 e8 cis'8 bes8 c'8 bes8
	~ bes16 aes16 a16 g16 bes16 a16 ~ a16 gis16 ~ gis8 f8
	gis,8 fis,8 ~ fis,16 ees,16 g,16 fis,16 ais,8 gis,8 ais16 g16
	bes16 a16 c'8 b8 cis'16 b16 dis'16 cis'16 ~ cis'4 g'4
}

PartPOneVoiceOne =  {
    \clef "treble_8" \key g \major \numericTimeSignature\time 4/4 g4 d'4
    d'4 c'8 [ b8 ] | % 2
    c'8 [ b8 ] a4 a4 g8 [ a8 ] | % 3
    b4 a8 [ b8 ] c'4 b8 [ g8 ] | % 4
    b4 a2. \break | % 5
    g4 d'4 d'4 c'8 [ b8 ] | % 6
    c'8 [ b8 ] a4 a4 g8 [ a8 ] | % 7
    b4 a8 [ b8 ] c'4 b8 [ g8 ] | % 8
    d'1 \break | % 9
    g4 d'4 d'4 c'8 [ b8 ] | \barNumberCheck #10
    c'8 [ b8 ] a4 a4 g8 [ a8 ] | % 11
    b4 a8 [ b8 ] c'4 b8 [ g8 ] | % 12
    b4 a4 a2 \break | % 13
    g4 d'4 d'4 c'8 [ b8 ] | % 14
    c'8 [ b8 ] a4 a4 g8 [ a8 ] | % 15
    \[ b4 \2 -0 b8 [ c'8 -1 ] d'4 ^\fermata -3 d'8 -1 [ e'8 -3 ] s2 \]
    g'2 ^\fermata -4 \bar "|."
    }

% The score definition
\score {
    <<
        \new Staff <<
            \set Staff.instrumentName = "Classical Guitar"
            \set Staff.shortInstrumentName = "Guit."
            \context Staff <<
                \context Voice = "PartPOneVoiceOne" { \PartPOneVoiceOne }
                >>
            >>



		\new Staff <<
			\set Staff.instrumentName = "Piano"
			\set Staff.shortInstrumentName = "Pno."
			\context Staff <<
				\context Voice = "CounterpointVoice" { \CounterpointVoice }
			>>
		>>

        >>
    \layout {}
     \midi {
		\tempo 4 = 120
	 }
    }
