\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{thesis}[2024/10/24 Class for UNIPI theses]

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{report}}

\ProcessOptions\relax

\PassOptionsToClass{a4paper,12pt,twoside,openright,english}{report}

\LoadClass{report}

\RequirePackage[utf8]{inputenc}
\RequirePackage{lmodern}
\RequirePackage[T1]{fontenc}
\RequirePackage[english]{babel}
\RequirePackage[autostyle=true]{csquotes}
\RequirePackage{microtype}
\RequirePackage{xcolor}
\RequirePackage{setspace}
\RequirePackage[a-3b,mathxmp]{pdfx}

\hypersetup{%
	bookmarksopen=true,%
	bookmarksnumbered,%
	hidelinks,%
	pdfstartview=,%
}

\RequirePackage[%
	a4paper,%
	twoside=true,%
	top=2.5cm,%
	bottom=2.5cm,%
	right=2.5cm,%
	left=3cm,%
	]{geometry}
\RequirePackage{fancyhdr}
\RequirePackage{xparse}
\RequirePackage{etoolbox}
\RequirePackage{suffix}
\RequirePackage{hyphenat}
\RequirePackage{latexsym}
\RequirePackage[export]{adjustbox}
\RequirePackage{minibox}
\RequirePackage{pdflscape}
\RequirePackage{graphicx}
\RequirePackage{float}
%\RequirePackage{subfig}
\RequirePackage{titlesec}
\RequirePackage[font=small,labelfont=bf,textfont=it,position=bottom]{caption}
\RequirePackage[justification=centering]{subcaption}
\RequirePackage{amsfonts}
\RequirePackage{amsmath}
\RequirePackage{amssymb}
\RequirePackage{siunitx}
\RequirePackage{tcolorbox}
\RequirePackage{interval}
\RequirePackage{array}
\RequirePackage{makecell}
\RequirePackage{booktabs}
\RequirePackage[inline]{enumitem}
\RequirePackage{multicol}
\RequirePackage{listings}
\RequirePackage{algorithm}
\RequirePackage{algpseudocode}
%\RequirePackage{minted}
\RequirePackage{emptypage}
\RequirePackage{nameref}
\RequirePackage[english]{varioref}
\RequirePackage[hyperref=true,
	url=true,
	isbn=true,
	backref=true,
	style=alphabetic,
	giveninits=true,
	sorting=nyt]{biblatex}
\RequirePackage{lipsum}
\RequirePackage{datetime2}

% maybe tabularx

\def\input@path{{./chapters/}}
\graphicspath{{./img/}}
\lstset{inputpath=./listings/}
\addbibresource{bibliography.bib}

\def\@institute{}
\def\@department{}
\def\@course{}
\def\@pretitle{}
\def\@supervisors{}
\def\@academicyear{}

\def\institute#1{\gdef\@institute{#1}}
\def\department#1{\gdef\@department{#1}}
\def\course#1{\gdef\@course{#1}}
\def\pretitle#1{\gdef\@pretitle{#1}}
\def\supervisors#1{\gdef\@supervisors{#1}}
\def\academicyear#1{\gdef\@academicyear{#1}}

\renewcommand{\maketitle}{%
	\begin{titlepage}
		\thispagestyle{titlepage}
		\singlespacing
		\begin{center}
			\includegraphics[width=0.9\textwidth]{img/marchio_unipi_pant541}\\
			\rule{\linewidth}{0.2mm}\\
			\ifx\@department\@empty \else
				{\LARGE\textsc{\@department}\\}
			\fi
			\ifx\@course\@empty \else
				{\Large\@course\\}
			\fi
			\vfill
			\ifx\@pretitle\@empty \else
				{\large\expandafter\MakeUppercase\expandafter{\@pretitle}\\}
			\fi
			\vspace{0.25cm}
			\ifx\@title\@empty \else
				{\LARGE\textbf{\expandafter\MakeUppercase\expandafter{\@title}}\\}
			\fi
			\vfill
			\large
			\begin{minipage}[t]{0.5\textwidth}
				\ifx\@supervisors\@empty \else{%
					SUPERVISORS\\[0.1cm]
					\textbf{\@supervisors}
				}\fi
			\end{minipage}
			\hfill
			\begin{minipage}[t]{0.49\textwidth}\raggedleft
				\ifx\@author\@empty \else{%
					CANDIDATE\\[0.1cm]
					\textbf{\@author}
				}\fi
			\end{minipage}
			\normalsize
			\vfill
			\rule{\linewidth}{0.2mm}\\[0.2cm]
			\ifx\@academicyear\@empty \else
				{\large\bfseries Academic Year \@academicyear}
			\fi
		\end{center}
	\end{titlepage}
}

\AtBeginEnvironment{verbatim}{\microtypesetup{activate=false}}

\definecolor{gray75}{gray}{0.75}

\definecolor{codegreen}{rgb}{0,0.6,0}
\definecolor{codegray}{rgb}{0.5,0.5,0.5}
\definecolor{codeorange}{rgb}{0.95,0.45,0.1}
\definecolor{codepurple}{rgb}{0.58,0,0.82}
\definecolor{codeblack}{rgb}{0.0,0.0,0.0}
\definecolor{bgcolor}{rgb}{0.95,0.95,0.92}

\definecolor{idle}{rgb}{0.5,0.5,0.5}
\definecolor{hearing}{rgb}{0,0,1}
\definecolor{collision}{rgb}{1,0,0}
\definecolor{relayed}{rgb}{0,1,0}

\titleformat{\chapter}[hang]{\Huge\bfseries}{\thechapter\hspace{20pt}\textcolor{gray75}{|}}{20pt}{}
%\titlespacing{\chapter}{0pt}{5ex}{4em}
\let\appendix@original\appendix
\renewcommand{\appendix}{\appendix@original\titleformat{\chapter}[display]{\Huge\bfseries}{Appendix~\thechapter}{20pt}{}}

\intervalconfig{soft open fences}

\newcommand\code{\texttt}
\newcommand\standout{\textbf}
\newcommand\specialname{\textsc}
\newcommand\idest[1]{(i.e.\ #1)}
\WithSuffix\newcommand\idest*[1]{i.e.\ #1}
\newcommand\exgratia[1]{(e.g.\ #1)}
\WithSuffix\newcommand\exgratia*[1]{e.g.\ #1}
\newcommand\etc{etc\textellipsis}
\newcommand\omnetpp{\specialname{OMNeT++}}
\newcommand\iblock{\code{iblock}}
\newcommand\inet{\specialname{inet}}

\newcommand*\elision{\textup{[\,\dots]}}

\newcommand\chref[1]{Chapter~\ref{#1}\space\enquote{\nameref{#1}}}
\newcommand\secref[1]{Section~\ref{#1}\space\enquote{\nameref{#1}}}
\newcommand\figref[1]{Figure~\ref{#1}}
\newcommand\lstref[1]{Listing~\ref{#1}}
\newcommand\tableref[1]{Table~\ref{#1}}
\newcommand\appendixref[1]{Appendix~\ref{#1}\space\enquote{\nameref{#1}}}
\let\eqref@original\eqref
\renewcommand{\eqref}[1]{Equation~\eqref@original{#1}}

\renewcommand\theadalign{bl}
\renewcommand\theadfont{\bfseries}
\renewcommand{\cellalign}{l}

\newcommand\theinstitute{\@institute}
\newcommand\thedepartment{\@department}
\newcommand\thecourse{\@course}
\newcommand\thepretitle{\@pretitle}
\newcommand\theacademicyear{\@academicyear}
\newcommand\thetitle{\@title}
\newcommand\theauthor{\@author}

\algdef{SE}[EVENT]{Event}{EndEvent}[1]{\textbf{on}\ #1}{\algorithmicend\ \textbf{event}}%
\algtext*{EndEvent}

\renewcommand\thealgorithm{\thechapter.\arabic{algorithm}}
\@addtoreset{algorithm}{chapter}

% \definecolor{codebg}{HTML}{FDF6E3}
% \definecolor{codecomment}{HTML}{93A1A1}
% \definecolor{codestring}{HTML}{2AA198}
% \definecolor{codekeyword}{HTML}{859900}
% \definecolor{codelinenum}{HTML}{808080}
% \colorlet{codenumber}{magenta!60!black}

% \lstdefinestyle{ccode}{
% 	backgroundcolor=\color{codebg},
% 	basicstyle=\footnotesize\ttfamily,
% 	commentstyle=\color{codecomment},
% 	stringstyle=\color{codestring},
% 	keywordstyle=\color{codekeyword},
% 	numberstyle=\tiny\color{codelinenum},
% 	captionpos=b,
% 	language=c,
% 	numbers=left,
% 	numbersep=5pt,
% 	stepnumber=1,
% 	tabsize=2,
% 	otherkeywords={float64_t},
% 	breaklines=true,
% 	breakatwhitespace=false,
% 	keepspaces=true,
% 	showspaces=false,
% 	showstringspaces=false,
% 	showtabs=false,
% 	%postbreak=\mbox{\textcolor{red}{$\hookrightarrow$}\space},
% 	literate=
% 		*{0}{{{\color{codenumber}0}}}{1}
% 		{1}{{{\color{codenumber}1}}}{1}
% 		{2}{{{\color{codenumber}2}}}{1}
% 		{3}{{{\color{codenumber}3}}}{1}
% 		{4}{{{\color{codenumber}4}}}{1}
% 		{5}{{{\color{codenumber}5}}}{1}
% 		{6}{{{\color{codenumber}6}}}{1}
% 		{7}{{{\color{codenumber}7}}}{1}
% 		{8}{{{\color{codenumber}8}}}{1}
% 		{9}{{{\color{codenumber}9}}}{1}
% }
%
% \lstset{style=ccode}

\setlength{\headheight}{14.49998pt}

\renewcommand{\chaptermark}[1]{%
\markboth{#1}{}}

\fancyhead{}
\fancyfoot{}
\fancyhead[LE]{\chaptername\ \thechapter\ --\ \leftmark}
\fancyhead[RO]{\chaptername\ \thechapter\ --\ \leftmark}
\fancyfoot[C]{\thepage}
\pagestyle{fancy}

\fancypagestyle{titlepage}{%
	\fancyhead{}%
	\fancyfoot{}%
	\fancyhead[L]{\texttt{DRAFT}}%
	\fancyhead[R]{\texttt{\DTMnow}}%
	\renewcommand{\headrulewidth}{0pt}%
}

\institute{University of Pisa}

\onehalfspacing

\endinput
