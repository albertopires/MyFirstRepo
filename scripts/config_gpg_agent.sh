#!/bin/bash

if [ -f "${HOME}/.gpg-agent-info" ]; then
	. "${HOME}/.gpg-agent-info"
	echo "GPG..."
	export GPG_AGENT_INFO
	export SSH_AUTH_SOCK
fi

