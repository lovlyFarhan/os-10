# .bashrc

# User specific aliases and functions

alias gits='git status'
alias gita='git add -A'
alias gitan='git add -An'
alias gitc='git commit -m'
alias gitpush='git push origin master'
alias gitpull='git pull origin master'


# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

PS1="\[\e[0;31m\][\u@\w]\[\e[0m\] "

