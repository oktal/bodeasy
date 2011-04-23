#include "exercisesdialog.h"
#include "ui_exercisesdialog.h"
#include "exercisesmodel.h"

#include <QWidget>
#include <QDialog>

ExercisesDialog::ExercisesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExercisesDialog),
    exercisesModel( new ExercisesModel( this ) )
{
    ui->setupUi(this);
    
    ui->lstExercises->setModel( exercisesModel );
    
    ui->bgType->setId( ui->rbRepetition, Exercise::Repetition );
    ui->bgType->setId( ui->rbDuration, Exercise::Duration );
    
    ui->cbDifficulty->addItem( tr( "Easy" ), Exercise::Easy );
    ui->cbDifficulty->addItem( tr( "Medium" ), Exercise::Medium );
    ui->cbDifficulty->addItem( tr( "Hard" ), Exercise::Hard );

}

ExercisesDialog::~ExercisesDialog()
{
    delete ui;
}

void ExercisesDialog::on_btnAdd_clicked()
{
    Exercise e( ui->leName->text(),
        Exercise::Type( ui->bgType->checkedId() ),
        Exercise::Difficulty( ui->cbDifficulty->itemData( ui->cbDifficulty->currentIndex() ).toInt() ),
        ui->cbUseWeight->isChecked(),
        ui->pteDescription->toPlainText() );
    
    if ( exercisesModel->addExercise( e ) ) {
        const QModelIndex index = exercisesModel->index( e );
        ui->lstExercises->setCurrentIndex( index );
    }
    else {
        qWarning( "Error" );
    }
}

void ExercisesDialog::on_btnModify_clicked()
{
}

void ExercisesDialog::on_btnDelete_clicked()
{
    exercisesModel->removeRow( ui->lstExercises->currentIndex().row() );
}
