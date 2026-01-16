using System.Windows;

namespace DW3ArduinoEditor.Views
{
   public partial class RenameTileMapWindow : Window
   {
      private string _oldTileMapName = string.Empty;

      public string NewTileMapName { get; private set; } = string.Empty;

      public RenameTileMapWindow( string oldTileMapName )
      {
         InitializeComponent();

         _oldTileMapName = oldTileMapName;
         NameTextBox.Text = _oldTileMapName;
      }

      private void CancelButton_Click( object sender, RoutedEventArgs e )
      {
         DialogResult = false;
         Close();
      }

      private void RenameButton_Click( object sender, RoutedEventArgs e )
      {
         if ( string.IsNullOrEmpty( NameTextBox.Text ) )
         {
            MessageBox.Show( "The tile map must have a name." );
            return;
         }

         NewTileMapName = NameTextBox.Text;
         DialogResult = true;
         Close();
      }
   }
}
