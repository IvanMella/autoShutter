# Impostazioni per accedere a gitHub via ssh
###1 Crea la coppia di chiavi SSH

        ssh-keygen -t ed25519 -C "your_email@example.com"

*crea la chiave. confermare il nome del file e la destinazione (~/.ssh/id_ed25519). Non mettere la passphrase.*

###2 Far partire l'agente ssh

        eval "$(ssh-agent -s)"

###3 Aggiungere la chiave all'agente.

        ssh-add ~/.ssh/id_ed25519


###4 Aggiungere a github la chiave pubblica
        cat ~/.ssh/id_ed25519.pub

*copia e incolla il contenuto ~/.ssh/id_ed25519 a github (settings -- ssh and gpg keys)*




###5 Verifica l'accesso:

        git remote -v
        > origin  https://github.com/OWNER/REPOSITORY.git (fetch)
        > origin  https://github.com/OWNER/REPOSITORY.git (push)

*cosi e' https*

###6 cambia l'accesso da https a ssh

        git remote set-url origin git@github.com:OWNER/REPOSITORY.git

Verifica di nuovo l'accesso:

        $ git remote -v
        > origin  git@github.com:OWNER/REPOSITORY.git (fetch)
        > origin  git@github.com:OWNER/REPOSITORY.git (push)

*cosi e' ssh*

fine.
