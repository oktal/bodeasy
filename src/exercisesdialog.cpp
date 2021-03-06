#include "exercisesdialog.h"
#include "ui_exercisesdialog.h"
#include "sql/models/exercisesmodel.h"
#include "sql/models/musclegroupsmodel.h"
#include "utils/modeltest/modeltest.h"

#include <QWidget>
#include <QDialog>
#include <QMessageBox>
#include <QItemSelectionModel>

#include <QDebug>

ExercisesDialog::ExercisesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExercisesDialog),
    exercisesModel( new ExercisesModel( this ) ),
    groupsModel( new MuscleGroupsModel( this ) )
{
    ui->setupUi(this);

//    new ModelTest(exercisesModel, this);
//    new ModelTest(groupsModel, this);
    
    ui->lstExercises->setModel( exercisesModel );
    
    ui->bgType->setId( ui->rbRepetition, Exercise::Repetition );
    ui->bgType->setId( ui->rbDuration, Exercise::Duration );
    
    ui->cbDifficulty->addItem( tr( "Facile" ), Exercise::Easy );
    ui->cbDifficulty->addItem( tr( "Moyen" ), Exercise::Medium );
    ui->cbDifficulty->addItem( tr( "Dur" ), Exercise::Hard );

    ui->cbGroup1->setModel( groupsModel );
    ui->cbGroup2->setModel( groupsModel );
    ui->cbGroup3->setModel( groupsModel );
    
    connect( ui->lstExercises->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( lstExercises_selectionModel_selectionChanged() ) );
}

ExercisesDialog::~ExercisesDialog()
{
    delete ui;
}

void ExercisesDialog::lstExercises_selectionModel_selectionChanged()
{
    const QModelIndex index = ui->lstExercises->selectionModel()->selectedIndexes().value( 0 );
    
    if ( !index.isValid() ) {
        return;
    }
    
    ui->btnDelete->setEnabled( true );
    ui->btnModify->setEnabled( true );
    const Exercise &e = exercisesModel->exercise( index );
    ui->leName->setText( e.name );
    switch ( e.type )
    {
        case Exercise::Repetition:
             ui->rbRepetition->setChecked( true );
             break;
        case Exercise::Duration:
            ui->rbDuration->setChecked( true );
            break;
    }
    ui->cbUseWeight->setChecked( e.useWeight );
    ui->cbDifficulty->setCurrentIndex( e.difficulty );
    ui->cbGroup1->setCurrentIndex( ui->cbGroup1->findData( e.groups[0] ) );
    if ( e.groups.size() >= 2 )
    {
        ui->chkGroup2->setEnabled( true );
        ui->chkGroup2->setChecked( true );

        ui->cbGroup2->setEnabled( true );
        ui->cbGroup2->setCurrentIndex( ui->cbGroup2->findData( e.groups[1] ) );

        ui->chkGroup3->setEnabled( true );
        ui->chkGroup3->setChecked( e.groups.size() == 3 );
        ui->cbGroup3->setEnabled( e.groups.size() == 3 );
        if ( e.groups.size() == 3 )
        {
            ui->cbGroup3->setCurrentIndex( ui->cbGroup3->findData( e.groups[2] ) );
        }
        else
        {
            ui->cbGroup3->setCurrentIndex( 0 );
        }
    } else {
        ui->chkGroup2->setChecked( false );
        ui->cbGroup2->setEnabled( false );
        ui->cbGroup2->setCurrentIndex( 0 );
        ui->chkGroup3->setEnabled( false );
        ui->cbGroup3->setCurrentIndex( 0 );
    }
    ui->pteDescription->setPlainText( e.description );
    mCurrentId = e.id;
}

void ExercisesDialog::on_leName_textEdited( const QString &text )
{
    ui->btnAdd->setEnabled( !text.isEmpty() );
    ui->btnModify->setEnabled( !text.isEmpty() && ui->lstExercises->selectionModel()->hasSelection() );
}

void ExercisesDialog::on_chkGroup2_clicked( bool checked )
{
    if ( !checked )
    {
        ui->cbGroup2->setCurrentIndex( 0 );
        ui->chkGroup3->setChecked( false );
        ui->cbGroup3->setCurrentIndex( 0 );
        ui->cbGroup3->setEnabled( false );
    }
}

void ExercisesDialog::on_chkGroup3_clicked( bool checked )
{
    if ( !checked )
    {
        ui->cbGroup3->setCurrentIndex( 0 );
    }
}

void ExercisesDialog::on_btnAdd_clicked()
{
    Exercise e( ui->leName->text(),
    Exercise::Type( ui->bgType->checkedId() ),
    Exercise::Difficulty( ui->cbDifficulty->itemData( ui->cbDifficulty->currentIndex() ).toInt() ),
    groups(),
    ui->cbUseWeight->isChecked(),
    ui->pteDescription->toPlainText() );

    if ( exercisesModel->index(e).isValid() ) {
        QMessageBox::critical(this, trUtf8( "Erreur" ),
                              trUtf8( "L'exercice que vous voulez ajouter existe déjà" ) );
        return;
    }

    if ( exercisesModel->addExercise( e ) ) {
        const QModelIndex index = exercisesModel->index( e );
        mCurrentId = e.id;
        ui->lstExercises->setCurrentIndex( index );
    }
    else {
        qWarning() << Q_FUNC_INFO << "Error";
    }
}

void ExercisesDialog::on_btnModify_clicked()
{
    Exercise e( ui->leName->text(),
    Exercise::Type( ui->bgType->checkedId() ),
    Exercise::Difficulty( ui->cbDifficulty->itemData( ui->cbDifficulty->currentIndex() ).toInt() ),
    groups(),
    ui->cbUseWeight->isChecked(),
    ui->pteDescription->toPlainText());

    e.id = mCurrentId;
    exercisesModel->updateExercise( e );
}

void ExercisesDialog::on_btnDelete_clicked()
{
    const QModelIndex index = ui->lstExercises->selectionModel()->selectedIndexes().value( 0 );
    exercisesModel->removeRow( index.row() );
}

QList<qint64> ExercisesDialog::groups() const
{
    QList<qint64> groupsList;
    groupsList.append( ui->cbGroup1->itemData( ui->cbGroup1->currentIndex() ).toLongLong() );
    if ( ui->chkGroup2->isChecked() )
    {
        groupsList.append( ui->cbGroup2->itemData( ui->cbGroup2->currentIndex() ).toLongLong() );
        if ( ui->chkGroup3->isChecked() )
        {
            groupsList.append( ui->cbGroup3->itemData( ui->cbGroup3->currentIndex() ).toLongLong() );
        }
    }

    return groupsList;
}
