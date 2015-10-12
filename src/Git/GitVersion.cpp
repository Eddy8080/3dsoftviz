#include "Git/GitVersion.h"

#include <QDebug>

#include "Git/GitFile.h"

Git::GitVersion::GitVersion() {
    this->changedFiles =  QList<Git::GitFile*>();
}

Git::GitVersion::GitVersion( QString commitId ) : commitId( commitId ) {
    this->changedFiles =  QList<Git::GitFile*>();
}

void Git::GitVersion::printVersion() {
    qDebug() << "CommitId : " << this->getCommitId();
    qDebug() << "Author : " << this->getAuthor();
    qDebug() << "Date : " << this->getDate();
    qDebug() << "List size : " << this->getChangedFiles().size();
    foreach( Git::GitFile* file , this->getChangedFiles() ) {
        qDebug() << file->getFilename() <<  " " << file->getFilepath() << " " << file->getTypeAsString();
    }
}

QList<Git::GitFile*> Git::GitVersion::getGitFilesByType( Git::GitType gitType ) {
    QList<Git::GitFile*> result = QList<Git::GitFile*>();
    foreach( Git::GitFile* gitFile, getChangedFiles() ) {
        if( gitFile->getType() == gitType ) {
            result.append( gitFile );
        }
    }

    return result;
}
