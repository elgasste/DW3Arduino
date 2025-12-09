using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Input;

namespace DW3ArduinoEditor
{
   public partial class AddNewTileMapWindow : Window
   {
      public string NewTileMapName { get; private set; } = string.Empty;
      public uint NewTilesX { get; private set; } = 0;
      public uint NewTilesY { get; private set; } = 0;
      public bool NewWraps { get; private set; } = false;
      public bool NewAffectsDaylight { get; private set; } = false;

      public AddNewTileMapWindow()
      {
         InitializeComponent();
      }

      [GeneratedRegex( "[^0-9]+" )]
      private static partial Regex NonNumericRegX();

      private void TileCount_PreviewTextInput( object sender, TextCompositionEventArgs e )
      {
         var regex = NonNumericRegX();
         e.Handled = regex.IsMatch( e.Text );
      }

      private void CancelButton_Click( object sender, RoutedEventArgs e )
      {
         DialogResult = false;
         Close();
      }

      private void CreateButton_Click( object sender, RoutedEventArgs e )
      {
         if ( string.IsNullOrEmpty( TileMapName.Text ) )
         {
            MessageBox.Show( "The tile map must have a name." );
            return;
         }
         else if ( !int.TryParse( HorizontalTilesTextBox.Text, out int tilesX ) )
         {
            MessageBox.Show( "Invalid value entered for horizontal tiles." );
            return;
         }
         else if ( tilesX < Constants.TileMapMinTilesX || tilesX > Constants.TileMapMaxTilesX )
         {
            MessageBox.Show( string.Format( "Horizontal tiles must be between {0} and {1}.", Constants.TileMapMinTilesX, Constants.TileMapMaxTilesX ) );
            return;
         }
         else if ( !int.TryParse( VerticalTilesTextBox.Text, out int tilesY ) )
         {
            MessageBox.Show( "Invalid value entered for vertical tiles." );
            return;
         }
         else if ( tilesY < Constants.TileMapMinTilesY || tilesY > Constants.TileMapMaxTilesY )
         {
            MessageBox.Show( string.Format( "Vertical tiles must be between {0} and {1}.", Constants.TileMapMinTilesY, Constants.TileMapMaxTilesY ) );
            return;
         }

         NewTileMapName = TileMapName.Text;
         NewTilesX = uint.Parse( HorizontalTilesTextBox.Text );
         NewTilesY = uint.Parse( VerticalTilesTextBox.Text );
         NewWraps = WrapsCheckBox.IsChecked ?? false;
         DialogResult = true;
         Close();
      }
   }
}
