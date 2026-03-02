package com.area.poc

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.ui.Modifier
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import com.area.poc.ui.screens.AREABuilderPage
import com.area.poc.ui.screens.HomePage
import com.area.poc.ui.screens.MyAreasPage
import com.area.poc.ui.theme.AREATheme
import com.area.poc.viewmodel.AREAViewModel

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            AREATheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    val navController = rememberNavController()
                    val viewModel: AREAViewModel = viewModel()
                    
                    NavHost(
                        navController = navController,
                        startDestination = "home"
                    ) {
                        composable("home") {
                            HomePage(
                                onCreateAreaClick = { navController.navigate("builder") },
                                onMyAreasClick = { navController.navigate("myareas") }
                            )
                        }
                        
                        composable("builder") {
                            AREABuilderPage(
                                viewModel = viewModel,
                                onNavigateToMyAreas = { 
                                    navController.navigate("myareas") {
                                        popUpTo("home")
                                    }
                                }
                            )
                        }
                        
                        composable("myareas") {
                            MyAreasPage(
                                viewModel = viewModel,
                                onNavigateToBuilder = { navController.navigate("builder") }
                            )
                        }
                    }
                }
            }
        }
    }
}
